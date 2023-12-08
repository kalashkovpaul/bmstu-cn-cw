Table patients {
  patient_id SERIAL [primary key]
  address_id INT
  passport INT
  phone VARCHAR(10)
  home_phone VARCHAR(10)
  email VARCHAR(20)
}

TABLE staff {
  staff_id SERIAL [PRIMARY KEY]
  passport INT
  post INT
  employment_date DATE
  dismissal_date DATE
}

TABLE posts {
  post_id SERIAL [PRIMARY KEY]
  title VARCHAR(30)
  salary INT
}

TABLE sessions{
  session_id SERIAL [PRIMARY KEY]
  doctor_id INT
  patient_id INT
  record_id INT
  state_id INT
  session_date DATE
  dynamics VARCHAR(256)
  prescription VARCHAR(256)
  note VARCHAR(256)
}

TABLE schedules{
  schedule_id SERIAL [PRIMARY KEY]
  staff_id INT
  workstart TIMESTAMP
  workend TIMESTAMP
  week_day week_day_t
  office INT
}

TABLE records{
  record_id SERIAL [PRIMARY KEY]
  agreement_id INT
  registration_date DATE
}

TABLE states{
  state_id SERIAL [PRIMARY KEY]
  general_condition VARCHAR(256)
  height INT
  patient_weight INT
  pulse INT
  pressure VARCHAR(50)
  temperature REAL
  other VARCHAR(256)
}

TABLE agreements{
  agreement_id SERIAL [PRIMARY KEY]
  patient_id INT
  code VARCHAR(50)
  conclusion_date DATE
  expiration_date DATE
  renewal_date DATE
}


TABLE addresses{
  address_id SERIAL [PRIMARY KEY]
  country VARCHAR(50)
  city VARCHAR(50)
  street VARCHAR(50)
  house VARCHAR(50)
  flat VARCHAR(50)
}

TABLE passports{
  passport_id SERIAL [PRIMARY KEY]
  surname VARCHAR(50)
  middlename VARCHAR(50)
  lastname VARCHAR(50)
  birth_date DATE
  gender gender_t
  series VARCHAR(10)
  num VARCHAR(10)
  issue_date DATE
  issue_location VARCHAR(128)
}

TABLE accesses{
  access_id SERIAL [PRIMARY KEY]
  staff_id INT
  username VARCHAR(50)
  passwordhash BYTEA
  access_level access_level_t
}

Ref: patients.patient_id < addresses.address_id

Ref: patients.passport - passports.passport_id

Ref: staff.post - posts.post_id

Ref: staff.passport - passports.passport_id

Ref: sessions.doctor_id > staff.staff_id

Ref: sessions.patient_id > patients.patient_id

Ref: sessions.record_id > records.record_id

Ref: sessions.state_id - states.state_id

Ref: schedules.staff_id > staff.staff_id

Ref: agreements.patient_id - patients.patient_id

Ref: accesses.staff_id - staff.staff_id
