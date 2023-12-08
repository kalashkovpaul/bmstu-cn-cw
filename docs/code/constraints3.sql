ALTER TABLE agreements ADD CONSTRAINT agr_patient_constraint
    FOREIGN KEY (patient_id) REFERENCES patients(patient_id)
    ON DELETE CASCADE;