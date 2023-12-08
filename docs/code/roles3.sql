GRANT all ON addresses TO chief;
GRANT all ON records TO chief;
GRANT all ON schedules TO chief;

GRANT all ON sessions TO doctor;
GRANT all ON records TO doctor;
GRANT all ON states TO doctor;
GRANT select ON patients TO doctor;
GRANT select ON staff TO doctor;
GRANT select, update ON schedules TO doctor;

GRANT all ON records TO registry;
GRANT all ON schedules TO registry;
GRANT all ON patients TO registry;
GRANT all ON passports TO registry;
GRANT select ON posts TO registry;
GRANT select ON staff TO registry;
