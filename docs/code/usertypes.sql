CREATE TYPE week_day_t as ENUM('Sunday', 'Monday', 'Tuesday',
    'Wednesday', 'Thursday', 'Friday', 'Saturday');

CREATE TYPE gender_t as ENUM('Male', 'Female', 'Other');

CREATE TYPE access_level_t as ENUM('admin', 'chief', 'doctor',
    'registry');
