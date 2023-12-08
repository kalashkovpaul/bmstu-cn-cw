ALTER TABLE patients ADD CONSTRAINT addr_constraint
    FOREIGN KEY (address_id) REFERENCES addresses(address_id)
    ON DELETE CASCADE;

ALTER TABLE accesses ADD CONSTRAINT agr_patient_constraint
    FOREIGN KEY (staff_id) REFERENCES staff(staff_id)
    ON DELETE CASCADE;

ALTER TABLE patients ADD CONSTRAINT pat_passport_constraint
    FOREIGN KEY (passport) REFERENCES passports(passport_id)
    ON DELETE CASCADE;