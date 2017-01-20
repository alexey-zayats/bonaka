-- init/tables/core.sql

CREATE SEQUENCE obj_id_seq;

-- obj
CREATE TABLE obj (
	id integer not null default nextval('obj_id_seq'),
	relname varchar,
	name varchar,
	guid uuid not null default uuid_generate_v4(),
	created timestamp not null default now(),
	updated timestamp not null default now()
);

CREATE TRIGGER _1_not_insert_update BEFORE INSERT OR UPDATE ON obj FOR EACH ROW EXECUTE PROCEDURE not_insert_update();

-- tree
CREATE TABLE tree (
	guid uuid not null default uuid_generate_v4(),
	pid integer,
	trail ltree not null,
	level integer
) INHERITS (obj);

CREATE TRIGGER _1_not_insert_update BEFORE INSERT OR UPDATE ON tree FOR EACH ROW EXECUTE PROCEDURE not_insert_update();

-- link
CREATE TABLE link (
	guid uuid not null default uuid_generate_v4(),
	src integer not null,
	dst integer not null
) inherits (obj);

CREATE TRIGGER _1_not_insert_update BEFORE INSERT OR UPDATE ON link FOR EACH ROW EXECUTE PROCEDURE not_insert_update();

-- role
CREATE TABLE role (
	guid uuid not null default uuid_generate_v4(),
	title varchar NOT NULL,
	description text NOT NULL DEFAULT ''
) INHERITS (obj);
	
COPY role ( id, relname, name, title) FROM stdin;
1	role	guest	Гость
2	role	user	Пользователь
3	role	manager	Менеджер
4	role	admin	Администратор
\.

CREATE TRIGGER role_obj_ins_up         BEFORE INSERT OR UPDATE          ON role FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update();
CREATE TRIGGER role_del                BEFORE DELETE                    ON role FOR EACH ROW EXECUTE PROCEDURE obj_before_delete();
CREATE TRIGGER role_tree_before_ins_up BEFORE INSERT OR UPDATE          ON role FOR EACH ROW EXECUTE PROCEDURE tree_before_insert_update();
CREATE TRIGGER role_tee_after_up       AFTER UPDATE                     ON role FOR EACH ROW EXECUTE PROCEDURE tree_after_update();

-- user
CREATE TABLE usr (
	guid uuid not null default uuid_generate_v4(),
	enabled  boolean NOT NULL DEFAULT TRUE,
	password   varchar NOT NULL DEFAULT random()::TEXT,
	firstname  varchar NOT NULL DEFAULT '',
	lastname   varchar NOT NULL DEFAULT '',
	middlename varchar NOT NULL DEFAULT '',
	email    varchar NOT NULL DEFAULT '',
	role     hstore NOT NULL DEFAULT '"user"=>"1"'
) INHERITS (obj);

COPY usr ( id, relname, name, firstname, lastname, enabled, email, password, role) FROM stdin;
10	usr	admin	admin	admin	true	admin@domain.ru	p@ssw0rd	"admin"=>"1"
11	usr	anonymous	anonymous	anonymous	true	anonymous@domain.ru	anonymous	"admin"=>"1"
\.

CREATE TRIGGER usr_obj_ins_up        BEFORE INSERT OR UPDATE          ON usr FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update();
CREATE TRIGGER usr_del               BEFORE DELETE                    ON usr FOR EACH ROW EXECUTE PROCEDURE obj_before_delete();

-- file
CREATE TABLE file (
  pid         int NOT NULL,
  lo          bytea NOT NULL,
  filename    varchar NOT NULL,
  mimetype    varchar NOT NULL DEFAULT 'application/octet-stream',
  size        integer NOT NULL DEFAULT 0
) INHERITS (obj);

CREATE TRIGGER file_ins_up BEFORE INSERT OR UPDATE ON file FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update();
CREATE TRIGGER file_del BEFORE DELETE ON file FOR EACH ROW EXECUTE PROCEDURE obj_before_delete();



