-- test.sql

insert into class ( name, title, pid, is_searchable, amodel) values
('news', 'Новости', (select id from class where name = 'obj'), true, 'table');

insert into attr (class_name, type_name, name, title, default_value, value_list, is_required, is_unique, is_array, is_searchable, referenced) 
values
('news', 'string', 'title', 'Заголовок', '', '{}', true, false, false, true, null),
('news', 'text', 'abstract', 'Анонс', '', '{}', true, false, false, true, null),
('news', 'text', 'content', 'Текст', '', '{}', true, false, false, true, null),
('news', 'datetime', 'start', 'Дата публикации', 'now()', '{}', true, false, false, true, null)
;
