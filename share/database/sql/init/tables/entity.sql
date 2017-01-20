

-- TYPE ----------------------------------------------------------------
-- BASE TYPES FOR MODEL
-- READ ONLY

CREATE TABLE type
(
	guid uuid not null default uuid_generate_v4(),
	title    varchar NOT NULL,                    -- LOCALIZED PRESENTS OF type varchar
	pgtype   varchar NOT NULL                    -- WHICH pgType WE MUST USE
) INHERITS ( obj );

COPY type ( id, relname, name, title, pgtype) FROM stdin;
100	type	int	Целое	integer
101	type	string	Строка	varchar
102	type	text	Текст	text
103	type	date	Дата	date
104	type	time	Время	time
105	type	datetime	Дата/Время	timestamp
106	type	list	Список	varchar[]
107	type	boolean	Логическое	boolean
108	type	double	Флоат	double precision
109	type	html	HTML	text
110	type	xml	XML	text
111	type	class	Класс	integer
112	type	trail	LTree	ltree
113	type	intarray	IntArray	integer[]
114	type	hstore	HStore	hstore
115	type	ttl	TTL	interval
116	type	bytea	Массив-байт	bytea
117	type	inet	network	inet
118	type	uuid	guid	uuid
\.

CREATE TRIGGER type_nowrite BEFORE INSERT OR UPDATE OR DELETE ON type FOR EACH ROW EXECUTE PROCEDURE not_insert_update();

-- CLASS ----------------------------------------------------------------

CREATE TABLE class
(
	guid uuid not null default uuid_generate_v4(),
	title varchar NOT NULL,
	is_searchable boolean DEFAULT false,
	amodel model NOT NULL DEFAULT 'list'
) INHERITS ( tree );

-- HERE A BASE CLASS'S DATA

COPY class ( id, relname, name, title, pid, trail, level, is_searchable, amodel) FROM stdin;
50	class	obj	Объект	50	obj	1	false	table
51	class	tree	Дерево	50	obj.tree	2	false	tree
54	class	role	Роль	50	obj.role	2	true	table
55	class	user	Пользователь	50	obj.user	2	true	table
56	class	link	Связь	50	obj.link	2	false	table
57	class	file	Файл	50	obj.file	2	true	table
\.

CREATE TRIGGER class_tree_before_ins_up     BEFORE INSERT OR UPDATE          ON class FOR EACH ROW EXECUTE PROCEDURE tree_before_insert_update();
CREATE TRIGGER class_class_after_ins_up_del AFTER INSERT OR UPDATE OR DELETE ON class FOR EACH ROW EXECUTE PROCEDURE class_after_ins_up_del();
CREATE TRIGGER class_obj_before_ins_up      BEFORE INSERT OR UPDATE          ON class FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update();
CREATE TRIGGER class_obj_delete             BEFORE DELETE                    ON class FOR EACH ROW EXECUTE PROCEDURE obj_before_delete();
CREATE TRIGGER class_tree_update            AFTER UPDATE                     ON class FOR EACH ROW EXECUTE PROCEDURE tree_after_update();
CREATE TRIGGER class_before_delete          BEFORE DELETE                    ON class FOR EACH ROW EXECUTE PROCEDURE class_before_delete();

-- ATTRIBUTE ----------------------------------------------------------------

CREATE TABLE attr
(
	guid uuid not null default uuid_generate_v4(),
	type_name varchar NOT NULL,
	class_name varchar NOT NULL,
	title varchar NOT NULL,
	default_value varchar,
	value_list	varchar[],
	is_attribute boolean DEFAULT false,
	is_required boolean DEFAULT false,
	is_unique boolean DEFAULT false,
	is_array boolean DEFAULT false,
	is_searchable boolean DEFAULT false,
	referenced varchar DEFAULT NULL
) INHERITS ( obj );

-- PUT HERE ATTRIBUTES FOR DEFAULT CLASS'S
COPY attr ( id, relname, class_name, type_name, name, title, default_value, value_list, is_required, is_unique, is_array, is_searchable, referenced ) FROM stdin;
200	attr	obj	int	id	Идентификатор	NULL	{}	t	t	f	f	null
201	attr	obj	string	relname	Класс объекта	NULL	{}	t	f	f	f	null
202	attr	obj	string	name	Имя	NULL	{}	t	f	f	t	null
203	attr	obj	uuid	guid	Глобальный Идентификатор	uuid_generate_v4()	{}	t	t	f	f	null
204	attr	obj	datetime	created	Дата создания	now()	{}	t	f	f	f	null
205	attr	obj	datetime	updated	Дата изменения	now()	{}	t	f	f	f	null
206	attr	tree	int	pid	Родитель	0	{}	t	f	f	f	null
207	attr	tree	trail	trail	Путь по дереву		{}	t	f	f	f	null
208	attr	tree	int	level	Уровень	1	{}	t	f	f	f	null
209	attr	role	string	title	Название		{}	t	f	f	t	null
210	attr	role	text	description	Описание		{}	t	f	f	t	null
211	attr	user	string	email	E-mail		{}	t	f	t	t	null
212	attr	user	string	password	Пароль		{}	t	f	t	f	null
213	attr	user	string	firstname	Имя		{}	t	f	t	t	null
214	attr	user	string	lastname	Фамилия		{}	t	f	t	t	null
215	attr	user	string	middlename	Отчество		{}	t	f	t	t	null
216	attr	user	hstore	role	Роли		{}	t	f	f	f	null
217	attr	link	class	src	Источник	0	{}	t	f	f	f	null
218	attr	link	class	dst	Назначение	0	{}	t	f	f	f	null
219	attr	file	class	pid	Родитель	0	{}	t	f	f	f	null
220	attr	file	string	filename	Имя файла		{}	t	f	f	t	null
221	attr	file	int	size	Размер файла	0	{}	t	f	f	f	null
222	attr	file	string	mimetype	Тип-mime	application/x-octet-stream	{}	t	f	f	f	null
223	attr	file	bytea	lo	Двоичный объект		{}	t	f	f	f	null
\.

CREATE TRIGGER attr_attribute_after_ins_up_del AFTER INSERT OR UPDATE OR DELETE ON attr
  FOR EACH ROW EXECUTE PROCEDURE attribute_after_ins_up_del();

CREATE TRIGGER attr_obj_before_ins_up BEFORE INSERT OR UPDATE ON attr
  FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update();

CREATE TRIGGER attr_delete BEFORE DELETE ON attr
  FOR EACH ROW EXECUTE PROCEDURE obj_before_delete();

CREATE TRIGGER attr_before_insert_update_delete BEFORE INSERT OR UPDATE OR DELETE ON attr
   FOR EACH ROW EXECUTE PROCEDURE attribute_before_insert_update();


