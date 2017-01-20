-- init/functions/core.sql

CREATE OR REPLACE FUNCTION not_insert_update () RETURNS TRIGGER AS $$
BEGIN
	IF TG_OP = 'INSERT' THEN
		RAISE EXCEPTION 'INSERT into table % not allowed', TG_RELNAME;
	ELSIF TG_OP = 'UPDATE' THEN
		RAISE EXCEPTION 'UPDATE of table % not allowed', TG_RELNAME;
	END IF;
	RETURN NULL;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION obj_before_insert_update () RETURNS TRIGGER AS $$
BEGIN
	NEW.relname = TG_RELNAME;
	NEW.updated := now();
	IF TG_OP = 'UPDATE' THEN
		IF NEW.id IS NULL THEN new.id := old.id; END IF; 
	ELSIF TG_OP = 'INSERT' THEN
		IF NEW.id IS NULL THEN NEW.id := nextval('obj_id_seq'); END IF;
		NEW.guid := uuid_generate_v4();
		IF NEW.created IS NULL THEN NEW.created := now(); END IF;
		IF NEW.name IS NULL THEN
    		NEW.name := TG_RELNAME || CAST(NEW.id as text);
		END IF;
	END IF;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;

-- Trigger function for DELETE in all tables (BEFORE DELETE)
-- warning! do not trigg this procedure on delete from deleted !
CREATE OR REPLACE FUNCTION obj_before_delete () RETURNS TRIGGER AS $$
DECLARE
  depends_on INT;
  rlm TEXT := TG_RELNAME;
BEGIN
    -- Delete all blobs and links attached to this object
    -- Restrict deletion when object is linked from another
    IF rlm NOT IN ('file', 'link') THEN
      IF EXISTS (SELECT * FROM link WHERE dst = old.id) THEN
        RAISE EXCEPTION 'Can not delete % % while it is linked from another object', rlm, old.id;
      END IF;
      DELETE FROM file WHERE pid = old.id;
      DELETE FROM link WHERE src = old.id;
    END IF;

  RETURN old;
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;

CREATE OR REPLACE FUNCTION tree_after_update () RETURNS TRIGGER AS $$
BEGIN
	IF NEW.trail <> OLD.trail THEN
		UPDATE tree SET trail = (NEW.trail || NEW.name) WHERE pid = NEW.id AND id <> NEW.id AND relname = TG_RELNAME;
	END IF;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION tree_after_delete () RETURNS TRIGGER AS $$
BEGIN
	DELETE FROM tree WHERE trail <@ OLD.trail;
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


-- Trigger function for INSERT or UPDATE in TREE (BEFORE INSERT/UPDATE)
CREATE OR REPLACE FUNCTION tree_before_insert_update () RETURNS TRIGGER AS $$
BEGIN
  IF NEW.pid = NEW.id OR NEW.pid = 0 THEN NEW.pid := NULL; END IF;
  IF NEW.trail IS NOT NULL AND NEW.pid IS NULL THEN
      SELECT INTO NEW.pid id FROM tree WHERE relname = TG_RELNAME AND trail @> NEW.trail ORDER BY trail DESC LIMIT 1;
      IF NEW.pid = NEW.id THEN NEW.pid := NULL; END IF;
  END IF;
  IF NEW.pid IS NULL THEN
      IF EXISTS (SELECT * FROM tree WHERE relname = TG_RELNAME AND pid IS NULL AND name = NEW.name AND id <> NEW.id) THEN
        RAISE EXCEPTION 'Root % with name "%" already exists', TG_RELNAME, NEW.name;
      END IF;
      IF NEW.trail IS NULL THEN
      	NEW.trail := text2ltree(NEW.name);
      END IF;
  ELSE
 --	IF NEW.trail IS NULL THEN
	    SELECT INTO NEW.trail trail FROM tree WHERE id = NEW.pid AND relname = TG_RELNAME;
	    IF NOT FOUND THEN
	      RAISE EXCEPTION 'Cannot find % % (for PID field)', TG_RELNAME, NEW.pid;
	    END IF;
	    NEW.trail = NEW.trail || NEW.name;
--	END IF;
    IF TG_OP = 'UPDATE' THEN
      IF NEW.pid <> OLD.pid THEN
        IF OLD.trail @> NEW.trail THEN
          RAISE EXCEPTION 'Cannot move % % into his subtree; % %', TG_RELNAME, NEW.id, OLD.trail, NEW.trail;
        END IF;
      END IF;
    END IF;
  END IF;
  NEW.level = nlevel(NEW.trail);
  RETURN NEW;
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;

CREATE OR REPLACE FUNCTION link_before_insert_update () RETURNS TRIGGER AS $$
DECLARE
	src integer;
	dst integer;
BEGIN
	src := 0;
	dst := 0;
	SELECT INTO src id FROM obj WHERE id = NEW.src;
	IF NOT FOUND THEN 
		RAISE EXCEPTION 'Cannot find src(%) for %', NEW.src, TG_RELNAME;
	END IF;

	SELECT INTO dst id FROM obj WHERE id = NEW.dst;
	IF NOT FOUND THEN 
		RAISE EXCEPTION 'Cannot find dst(%) for %', NEW.dst, TG_RELNAME;
	END IF;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;


