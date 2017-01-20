
---------------------- CLASS & ATTRIBUTES MANAGER -------------------------

CREATE OR REPLACE FUNCTION class_before_delete() RETURNS TRIGGER AS $$
DECLARE
BEGIN
	EXECUTE 'DELETE FROM attr WHERE class_name = ' || quote_literal(OLD.name);
	RETURN OLD;
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;

CREATE OR REPLACE FUNCTION class_after_ins_up_del() RETURNS TRIGGER AS $$
DECLARE
	parent RECORD;
	attrs RECORD;
	frealm varchar := 'class';
BEGIN
-- 	raise notice 'class trigger';
	IF (TG_OP = 'INSERT') THEN
		SELECT INTO parent name, trail FROM class WHERE id = NEW.pid;

		-- 1. create table
		EXECUTE 'CREATE TABLE ' || lower( NEW.name ) || ' (
			) INHERITS ( ' || lower( parent.name ) || ' )';

		EXECUTE 'ALTER TABLE ONLY '|| lower( NEW.name ) || ' ALTER COLUMN relname SET DEFAULT ' || quote_literal( NEW.name );
		-- 2 create PK
		EXECUTE 'ALTER TABLE ONLY ' || lower( NEW.name ) || ' ADD CONSTRAINT ' || lower( NEW.name ) || '_id_pkey PRIMARY KEY (id)';
		-- 3 create indexes
		EXECUTE 'CREATE INDEX ' || lower( NEW.name ) || '_id_key ON ' || lower( NEW.name ) || '( id ) ';
		EXECUTE 'CREATE INDEX ' || lower( NEW.name ) || '_name_key ON ' || lower( NEW.name ) || '( name ) ';
		EXECUTE 'CREATE INDEX ' || lower( NEW.name ) || '_relname_key ON ' || lower( NEW.name ) || '( relname ) ';
		EXECUTE 'CREATE INDEX ' || lower( NEW.name ) || '_created_key ON ' || lower( NEW.name ) || '( created ) ';
		EXECUTE 'CREATE INDEX ' || lower( NEW.name ) || '_updated_key ON ' || lower( NEW.name ) || '( updated ) ';
		
		IF NEW.trail <@ 'obj.tree' THEN
			EXECUTE 'CREATE INDEX ' || lower( NEW.name ) || '_pid_key ON ' || lower( NEW.name ) || '( pid ) ';
			EXECUTE 'CREATE INDEX ' || lower( NEW.name ) || '_trail_key ON ' || lower( NEW.name ) || ' USING gist ( trail ) ';
			EXECUTE 'CREATE INDEX ' || lower( NEW.name ) || '_level_key ON ' || lower( NEW.name ) || ' ( level ) ';
			
			EXECUTE 'CREATE TRIGGER ' || lower( NEW.name ) || '_tree_before_insert_update BEFORE INSERT OR UPDATE ON '
				|| lower( NEW.name ) || ' FOR EACH ROW EXECUTE PROCEDURE tree_before_insert_update()';
				
			EXECUTE 'CREATE TRIGGER ' || lower( NEW.name ) || '_tree_after_update AFTER UPDATE ON '
				|| lower( NEW.name ) || ' FOR EACH ROW EXECUTE PROCEDURE tree_after_update()';
		END IF;
		
		EXECUTE 'CREATE TRIGGER ' || lower( NEW.name ) || '_obj_before_insert_update BEFORE INSERT OR UPDATE ON '
			|| lower( NEW.name ) || ' FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update()';

		EXECUTE 'CREATE TRIGGER ' || lower( NEW.name ) || '_obj_before_delete BEFORE DELETE ON '
			|| lower( NEW.name ) || ' FOR EACH ROW EXECUTE PROCEDURE obj_before_delete()';

		-- 3. LOOPING for every attributes on class
		FOR attrs IN
			SELECT
				cast( a.name::varchar as text) as _name,
				a.default_value,
				a.is_array,
				a.is_unique,
				a.is_searchable,
				a.referenced,
				a.id, 
				a.type_name as type_name,
				t.pgtype as type_pgtype
			FROM
				attr AS a,
				class AS c,
				type AS t
			WHERE
				c.trail @> NEW.trail
				AND a.class_name = c.name
				AND t.name = a.type_name 
		LOOP

			-- raise notice 'ATTR DEFAULT: % % %', attrs._name, attrs.default_value, attrs.default_value is null;

			IF right(attrs.default_value::text, 2) != '()' AND attrs.default_value != 'NULL' THEN
				attrs.default_value := '''' || attrs.default_value || ''''; 
			END IF;
			
			EXECUTE 'ALTER TABLE ' || lower(NEW.name) || ' ALTER ' || attrs._name || ' SET DEFAULT ' || attrs.default_value;

			IF ( attrs.type_name = 'class' AND length(attrs.referenced) > 0 ) THEN
				SELECT INTO frealm name FROM class WHERE name = attrs.referenced;
				EXECUTE 'ALTER TABLE ONLY ' || NEW.name
					|| ' ADD CONSTRAINT ' || NEW.name || '_' || attrs._name || '_fkey FOREIGN KEY (' || attrs._name || ')'
					|| ' REFERENCES ' || frealm || '(id) ON DELETE CASCADE';
			END IF;

		END LOOP;
		
		return NEW;
	ELSIF (TG_OP = 'UPDATE') THEN
		-- TODO: implement update
		IF ( NEW.name != OLD.name ) THEN
			RAISE EXCEPTION '%(%): change class name not implemented', TG_NAME, TG_OP;
		END IF;
		IF ( NEW.up != OLD.up ) THEN
			RAISE EXCEPTION '%(%): change class parent not implemented', TG_NAME, TG_OP;
		END IF;
		IF ( NEW.trail != OLD.trail ) THEN
			RAISE EXCEPTION '%(%): change class parentless not implemented', TG_NAME, TG_OP;
		END IF;
		return NEW;
	ELSIF (TG_OP = 'DELETE') THEN
		EXECUTE 'DROP INDEX ' || lower( OLD.name ) || '_id_key';
		EXECUTE 'DROP INDEX ' || lower( OLD.name ) || '_name_key';
		EXECUTE 'DROP INDEX ' || lower( OLD.name ) || '_created_key';
		EXECUTE 'DROP INDEX ' || lower( OLD.name ) || '_updated_key';
		
		IF OLD.trail <@ 'obj.tree' THEN
			EXECUTE 'DROP INDEX ' || lower( OLD.name ) || '_pid_key';
			EXECUTE 'DROP INDEX ' || lower( OLD.name ) || '_trail_key';
			EXECUTE 'DROP INDEX ' || lower( OLD.name ) || '_level_key';
			
			EXECUTE 'DROP TRIGGER ' || lower( OLD.name ) || '_tree_before_insert_update ON ' || lower( OLD.name );
			EXECUTE 'DROP TRIGGER ' || lower( OLD.name ) || '_tree_after_update ON '	|| lower( OLD.name );
		END IF;

		EXECUTE 'DROP TRIGGER ' || lower( OLD.name ) || '_obj_before_insert_update ON ' || lower( OLD.name );
		EXECUTE 'DROP TRIGGER ' || lower( OLD.name ) || '_obj_before_delete ON ' || lower( OLD.name );

		-- drop PK
		EXECUTE 'ALTER TABLE ONLY ' || lower(OLD.name) || ' DROP CONSTRAINT ' || lower(OLD.name) || '_id_pkey';
		-- 1. DROP realm
		EXECUTE 'DROP TABLE ' || lower( OLD.name ) || ' CASCADE';
		return OLD;
	END IF;
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;


CREATE OR REPLACE FUNCTION attribute_before_insert_update() RETURNS TRIGGER AS $$
DECLARE
	rec_class RECORD;
	rec_type RECORD;
BEGIN
	IF (TG_OP = 'INSERT' OR TG_OP = 'UPDATE') THEN
		SELECT INTO rec_type * FROM type WHERE name = NEW.type_name;
		SELECT INTO rec_class * FROM class WHERE name = NEW.class_name;

		IF rec_type IS NULL THEN
			raise EXCEPTION 'Type not found: %', NEW.type_name;
		END IF;

		IF rec_class IS NULL THEN
			raise EXCEPTION 'Class not found: %', NEW.class_name;	
		END IF;
		return NEW;
	ELSIF (TG_OP = 'DELETE') THEN
		SELECT INTO rec_class * FROM class WHERE name = OLD.class_name;
		IF rec_class IS NULL THEN
			raise EXCEPTION 'Class not found: % ', NEW.type_name;	
		END IF;
		return OLD;
	END IF;
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;

CREATE OR REPLACE FUNCTION attribute_after_ins_up_del() RETURNS TRIGGER AS $$
DECLARE
	rec_class RECORD;
	rec_type RECORD;
	attrs RECORD;
	sql	varchar := '';
BEGIN
	-- RAISE NOTICE '%(%)', TG_NAME, TG_OP;
	IF (TG_OP = 'INSERT') THEN
		SELECT INTO rec_type * FROM type WHERE name = NEW.type_name;
		SELECT INTO rec_class * FROM class WHERE name = NEW.class_name;

		sql := 'ALTER TABLE ' || lower(rec_class.name) || ' ADD COLUMN '
				|| lower(NEW.name) || ' ' || lower(rec_type.pgtype);

		IF ( NEW.is_array ) THEN
			sql := sql || '[]';
		END IF;

		IF ( NEW.is_required ) THEN
			sql := sql || ' NOT NULL';
		END IF;

		IF ( NEW.is_unique ) THEN
			sql := sql || ' UNIQUE';
		END IF;

		IF ( NEW.default_value IS NULL ) THEN
			sql := sql || ' DEFAULT NULL';
		ELSIF right(NEW.default_value::text, 2) != '()' THEN
			sql := sql || ' DEFAULT ''' || NEW.default_value || '''';
		ELSE
			sql := sql || ' DEFAULT ' || NEW.default_value;
		END IF;

		-- raise notice 'QUERY: %', sql;
		EXECUTE sql;
			
		EXECUTE 'CREATE INDEX ' || rec_class.name || '_' || NEW.name || '_key ON ' || rec_class.name || '(' || NEW.name || ')';

		IF ( rec_type.name = 'class' AND length(NEW.referenced) > 0 ) THEN
			SELECT INTO sql name FROM class WHERE name = NEW.referenced;
			IF FOUND THEN
				EXECUTE 'ALTER TABLE ONLY ' || rec_class.name
					|| ' ADD CONSTRAINT ' || rec_class.name || '_' || NEW.name || '_fkey FOREIGN KEY (' || NEW.name || ')'
					|| ' REFERENCES ' || NEW.referenced || '(id) ON DELETE CASCADE';
			END IF;
		END IF;

		RETURN NEW;
	ELSIF (TG_OP = 'UPDATE') THEN
		-- TODO: implement update
		RAISE EXCEPTION 'Update is not implemented: %(%)', TG_NAME, TG_OP;
	ELSIF (TG_OP = 'DELETE') THEN
		SELECT INTO rec_class * FROM class WHERE name = OLD.class_name;
	
		EXECUTE 'ALTER TABLE ' || rec_class.name || ' DROP COLUMN ' || OLD.name;

		RETURN OLD;
	END IF;
	
	RETURN NULL;
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;

