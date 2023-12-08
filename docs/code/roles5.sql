CREATE VIEW session AS
SELECT current_setting('app.user_uuid')::int AS user_uuid;

GRANT all ON session TO admin, chief, doctor, registry;

CREATE POLICY users_select
    ON accesses
    FOR SELECT
    USING (true);

CREATE POLICY users_update
    ON accesses
    FOR UPDATE
    TO doctor, chief, registry
    USING (access_id = (select user_uuid
                   from session));

CREATE POLICY users_update_admin
    ON accesses
    FOR UPDATE
    TO admin
    USING (true);

CREATE POLICY users_delete_admin
    ON accesses
    FOR DELETE
    TO admin
    USING (true);