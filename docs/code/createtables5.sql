CREATE TABLE accesses (
    access_id SERIAL NOT NULL PRIMARY KEY,
    staff_id INT NOT NULL,
    username VARCHAR(50) NOT NULL,
    passwordhash BYTEA NOT NULL,
    access_level access_level_t NOT NULL
);