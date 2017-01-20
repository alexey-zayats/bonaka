-- init/index/entity.sql

CREATE INDEX type_id_key          ON type( id );
CREATE INDEX type_name_key        ON type( name );
CREATE UNIQUE INDEX type_uniq_name_key   ON type( name );
CREATE INDEX type_realm_key       ON type( relname );
CREATE INDEX type_created_key     ON type( created );
CREATE INDEX type_updated_key     ON type( updated );
CREATE INDEX type_title_key       ON type( title );
CREATE INDEX type_pgtype_key      ON type( pgtype );
ALTER TABLE type ADD PRIMARY KEY (id);

CREATE INDEX class_id_key          ON class( id );
CREATE INDEX class_name_key        ON class( name );
CREATE UNIQUE INDEX class_uniq_name_key   ON class( name );
CREATE INDEX class_realm_key       ON class( relname );
CREATE INDEX class_created_key     ON class( created );
CREATE INDEX class_updated_key     ON class( updated );
CREATE INDEX class_trail_key       ON class USING gist ( trail );
CREATE INDEX class_pid_key         ON class ( pid );
CREATE INDEX class_level_key       ON class ( level );
CREATE INDEX class_title_key       ON class ( title );
ALTER TABLE class ADD PRIMARY KEY (id);

CREATE INDEX attr_id_key ON attr( id );
CREATE INDEX attr_realm_key ON attr( relname );
CREATE INDEX attr_created_key ON attr( created );
CREATE INDEX attr_updated_key ON attr( updated );
CREATE INDEX attr_type_name_key ON attr( type_name );
CREATE INDEX attr_class_name_key ON attr ( class_name );
CREATE INDEX attr_title_key ON attr( title );
CREATE INDEX attr_default_value_key ON attr( default_value );
CREATE INDEX attr_value_list_key ON attr ( value_list );
CREATE INDEX attr_is_attribute_key ON attr( is_attribute );
CREATE INDEX attr_is_required_key ON attr( is_required );
CREATE INDEX attr_is_unique_key ON attr( is_unique );
CREATE INDEX attr_is_array_key ON attr( is_array );
CREATE INDEX attr_is_searchable_key ON attr( is_searchable );
ALTER TABLE attr ADD PRIMARY KEY (id);
ALTER TABLE attr ADD CONSTRAINT attr_type_name_fk FOREIGN KEY (type_name) REFERENCES type (name);
ALTER TABLE attr ADD CONSTRAINT attr_class_name_fk FOREIGN KEY (class_name) REFERENCES class (name);
