-- init/index/core.sql

CREATE INDEX obj_id_key           ON obj( id );
CREATE INDEX obj_name_key         ON obj( name );
CREATE INDEX obj_relname_key      ON obj( relname );
CREATE INDEX obj_created_key      ON obj( created );
CREATE INDEX obj_updated_key      ON obj( updated );
ALTER TABLE obj ADD PRIMARY KEY (id);

CREATE INDEX tree_id_key          ON tree( id );
CREATE INDEX tree_name_key        ON tree( name );
CREATE INDEX tree_relname_key     ON tree( relname );
CREATE INDEX tree_created_key     ON tree( created );
CREATE INDEX tree_updated_key     ON tree( updated );
CREATE INDEX tree_trail_key       ON tree USING gist ( trail );
CREATE INDEX tree_pid_key         ON tree ( pid );
CREATE INDEX tree_level_key       ON tree ( level );
ALTER TABLE tree ADD PRIMARY KEY (id);

CREATE INDEX link_id_key          ON link( id );
CREATE INDEX link_name_key        ON link( name );
CREATE INDEX link_relname_key     ON link( relname );
CREATE INDEX link_created_key     ON link( created );
CREATE INDEX link_updated_key     ON link( updated );
CREATE INDEX link_src_key         ON link( src );
CREATE INDEX link_dst_key         ON link( dst );
ALTER TABLE link ADD PRIMARY KEY (id);

CREATE INDEX role_id_key          ON role( id );
CREATE INDEX role_name_key        ON role( name );
CREATE INDEX role_relname_key     ON role( relname );
CREATE INDEX role_created_key     ON role( created );
CREATE INDEX role_updated_key     ON role( updated );
ALTER TABLE role ADD PRIMARY KEY (id);

CREATE INDEX usr_id_key           ON usr( id );
CREATE INDEX usr_name_key         ON usr( name );
CREATE INDEX usr_relname_key      ON usr( relname );
CREATE INDEX usr_created_key      ON usr( created );
CREATE INDEX usr_updated_key      ON usr( updated );
CREATE INDEX usr_role_idx         ON usr USING gist (role);
ALTER TABLE usr ADD PRIMARY KEY (id);

CREATE INDEX file_id_key          ON file( id );
CREATE INDEX file_name_key        ON file( name );
CREATE INDEX file_relname_key     ON file( relname );
CREATE INDEX file_created_key     ON file( created );
CREATE INDEX file_updated_key     ON file( updated );
CREATE INDEX file_pid_key         ON file( pid );
CREATE INDEX file_mimetype_key    ON file( mimetype );
ALTER TABLE file ADD PRIMARY KEY (id);

