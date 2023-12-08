CREATE OR REPLACE PROCEDURE before_each_query(IN user_uuid int)
    LANGUAGE plpgsql
AS
$$
DECLARE
    role_name regrole;
BEGIN
    role_name = (SELECT role FROM users WHERE
        accesses.access_id = user_uuid)::regrole;
    execute format('SET role %I', role_name);
    execute format('SET app.user_uuid = %L', user_uuid);
END;
$$;