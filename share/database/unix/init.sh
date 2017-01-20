#!/bin/bash

.  .pgenv

echo drop
$PSQL -qf ../sql/init/drop.sql

echo types
$PSQL -qf ../sql/init/types/core.sql

echo functions
$PSQL -qf ../sql/init/functions/util.sql
$PSQL -qf ../sql/init/functions/core.sql
$PSQL -qf ../sql/init/functions/entity.sql
$PSQL -qf ../sql/init/functions/rbac.sql

echo tables
$PSQL -qf ../sql/init/tables/core.sql
$PSQL -qf ../sql/init/tables/entity.sql
$PSQL -qf ../sql/init/tables/rbac.sql
$PSQL -qf ../sql/init/tables/bonaka.sql

echo data
$PSQL -qf ../sql/data/core.sql
$PSQL -qf ../sql/data/entity.sql
$PSQL -qf ../sql/data/rbac.sql
$PSQL -qf ../sql/data/bonaka.sql

echo indexes
$PSQL -qf ../sql/init/index/core.sql
$PSQL -qf ../sql/init/index/entity.sql
$PSQL -qf ../sql/init/index/rbac.sql
$PSQL -qf ../sql/init/index/bonaka.sql
