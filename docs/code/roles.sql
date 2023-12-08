CREATE EXTENSION IF NOT EXISTS "uuid-ossp";
CREATE ROLE admin;
CREATE ROLE chief;
CREATE ROLE doctor;
CREATE ROLE registry;

SET ROLE superuser;
DROP OWNED BY admin;
DROP OWNED BY chief;
DROP OWNED BY doctor;
DROP OWNED BY registry;

grant all on all tables in schema public to admin;

grant all on sessions to chief;
grant all on staff to chief;
grant all on states to chief;
grant all on posts to chief;
grant all on patients to chief;
grant all on passports to chief;
grant all on agreements to chief;
grant all on addresses to chief;
grant all on records to chief;
grant all on schedules to chief;

grant all on sessions to doctor;
grant all on records to doctor;
grant all on states to doctor;
grant select on patients to doctor;
grant select on staff to doctor;
grant select, update on schedules to doctor;

grant all on records to registry;
grant all on schedules to registry;
grant all on patients to registry;
grant all on passports to registry;
grant select on posts to registry;
grant select on staff to registry;

ALTER TABLE accesses
    ENABLE ROW LEVEL SECURITY;

DROP VIEW IF EXISTS session;
CREATE VIEW session AS
SELECT current_setting('app.user_uuid')::int AS user_uuid;

grant all on session to admin, chief, doctor, registry;

DROP POLICY IF EXISTS users_select ON users;
CREATE POLICY users_select
    ON accesses
    FOR SELECT
    USING (true);

DROP POLICY IF EXISTS users_update ON users;
CREATE POLICY users_update
    ON accesses
    FOR UPDATE
    TO doctor, chief, registry
    USING (access_id = (select user_uuid
                   from session));

DROP POLICY IF EXISTS users_update_admin ON users;
CREATE POLICY users_update_admin
    ON accesses
    FOR UPDATE
    TO admin
    USING (true);

DROP POLICY IF EXISTS users_delete_admin ON users;
CREATE POLICY users_delete_admin
    ON accesses
    FOR DELETE
    TO admin
    USING (true);

    DROP PROCEDURE IF EXISTS before_each_query(user_uuid int);
CREATE OR REPLACE PROCEDURE before_each_query(IN user_uuid int)
    LANGUAGE plpgsql
AS
$$
DECLARE
    role_name regrole;
BEGIN
    role_name = (SELECT role FROM users WHERE accesses.access_id = user_uuid)::regrole;
    execute format('SET role %I', role_name);
    execute format('SET app.user_uuid = %L', user_uuid);
END;
$$;
