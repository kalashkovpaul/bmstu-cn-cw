ALTER TABLE patients ADD CONSTRAINT addr_constraint
    FOREIGN KEY (address_id) REFERENCES addresses(address_id)
    ON DELETE CASCADE;

ALTER TABLE patients ADD CONSTRAINT pat_passport_constraint
    FOREIGN KEY (passport) REFERENCES passports(passport_id)
    ON DELETE CASCADE;

ALTER TABLE staff ADD CONSTRAINT staff_passport_constraint
    FOREIGN KEY (passport) REFERENCES passports(passport_id)
    ON DELETE CASCADE;

ALTER TABLE staff ADD CONSTRAINT staff_post_constraint
    FOREIGN KEY (post) REFERENCES posts(post_id)
    ON DELETE CASCADE;

ALTER TABLE sessions ADD CONSTRAINT session_doctor_contraint
    FOREIGN KEY (doctor_id) REFERENCES staff(staff_id)
    ON DELETE CASCADE;

ALTER TABLE sessions ADD CONSTRAINT session_patient_contraint
    FOREIGN KEY (patient_id) REFERENCES patients(patient_id)
    ON DELETE CASCADE;

ALTER TABLE sessions ADD CONSTRAINT session_record_constraint
    FOREIGN KEY (record_id) REFERENCES records(record_id)
    ON DELETE CASCADE;

ALTER TABLE sessions ADD CONSTRAINT session_state_constraint
    FOREIGN KEY (state_id) REFERENCES states(state_id)
    ON DELETE CASCADE;

ALTER TABLE schedules ADD CONSTRAINT schedule_staff_constraint
    FOREIGN KEY (staff_id) REFERENCES staff(staff_id)
    ON DELETE CASCADE;

ALTER TABLE agreements ADD CONSTRAINT agreement_patient_constraint
    FOREIGN KEY (patient_id) REFERENCES patients(patient_id)
    ON DELETE CASCADE;

ALTER TABLE accesses ADD CONSTRAINT agreement_patient_constraint
    FOREIGN KEY (staff_id) REFERENCES staff(staff_id)
    ON DELETE CASCADE;