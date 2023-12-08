import Fastify, {FastifyInstance} from 'fastify';
import { apiConfig } from '../configs/api.config';
import { patientInfoHandler } from '../handlers/PatientInfoHandler';
import { authHandler } from '../handlers/AuthHandler';
import { patientHandler } from '../handlers/PatientHandler';
import { addPostHandler } from '../handlers/AddPostHandler';
import { allPostsHandler } from '../handlers/AllPostsHandler';
import { updatePostHandler } from '../handlers/UpdatePostHandler';
import { addStaffHandler } from '../handlers/AddStaffHandler';
import { updateStaffHandler } from '../handlers/UpdateStaffHandler';
import { staffShortInfoHandler } from '../handlers/StaffShortInfoHandler';
import { staffFullInfoHandler } from '../handlers/StaffFullInfoHandler';
import { dismissStaffHandler } from '../handlers/DismissStaffHandler';
import { addUserHandler } from '../handlers/AddUserHandler';
import { updateUserHandler } from '../handlers/UpdateUserHandler';
import { deleteUserHandler } from '../handlers/DeleteUserHandler';
import { userShortInfoHandler } from '../handlers/UserShortInfoHandler';
import { scheduleHandler } from '../handlers/ScheduleHandler';
import { allScheduleHandler } from '../handlers/AllScheduleHandler';
import { newRecordHandler } from '../handlers/NewRecordHandler';
import { currentRecordHandler } from '../handlers/CurrentRecordHandler';
import { recordShortInfoHandler } from '../handlers/RecordShortInfoHandler';
import { recordFullInfoHandler } from '../handlers/RecordFullInfoHandler';
import { agreementHandler } from '../handlers/AgreementHandler';

export class Server {
    private api: FastifyInstance;

    constructor() {
        this.api = Fastify();
        this.api.get(apiConfig.patientFullInfo, patientInfoHandler.request.bind(patientInfoHandler));
        this.api.put(apiConfig.patientFullInfo, patientInfoHandler.request.bind(patientInfoHandler));
        this.api.post(apiConfig.patientFullInfo, patientInfoHandler.request.bind(patientInfoHandler));
        this.api.post(apiConfig.postAuth, authHandler.request.bind(authHandler));
        this.api.get(apiConfig.patientShortInfo, patientHandler.request.bind(patientHandler));
        this.api.post(apiConfig.addPost, addPostHandler.request.bind(addPostHandler));
        this.api.get(apiConfig.allPosts, allPostsHandler.request.bind(allPostsHandler));
        this.api.put(apiConfig.updatePost, updatePostHandler.request.bind(updatePostHandler));
        this.api.post(apiConfig.addStaff, addStaffHandler.request.bind(addStaffHandler));
        this.api.put(apiConfig.updateStaff, updateStaffHandler.request.bind(updateStaffHandler));
        this.api.get(apiConfig.staffShort, staffShortInfoHandler.request.bind(staffShortInfoHandler));
        this.api.get(apiConfig.fullStaffInfo, staffFullInfoHandler.request.bind(staffFullInfoHandler));
        this.api.post(apiConfig.dismissStaff, dismissStaffHandler.request.bind(dismissStaffHandler));
        this.api.post(apiConfig.addUser, addUserHandler.request.bind(addUserHandler));
        this.api.post(apiConfig.updateUser, updateUserHandler.request.bind(updateUserHandler));
        this.api.post(apiConfig.deleteUser, deleteUserHandler.request.bind(deleteUserHandler));
        this.api.get(apiConfig.systemUsersList, userShortInfoHandler.request.bind(userShortInfoHandler));
        this.api.post(apiConfig.postSchedule, scheduleHandler.request.bind(scheduleHandler));
        this.api.get(apiConfig.allSchedules, allScheduleHandler.request.bind(allScheduleHandler));
        this.api.post(apiConfig.newRecord, newRecordHandler.request.bind(newRecordHandler));
        this.api.post(apiConfig.currentRecords, currentRecordHandler.request.bind(currentRecordHandler));
        this.api.get(apiConfig.shortRecords, recordShortInfoHandler.request.bind(recordShortInfoHandler));
        this.api.post(apiConfig.recordFull, recordFullInfoHandler.request.bind(recordFullInfoHandler));
        this.api.get(apiConfig.agreement, agreementHandler.request.bind(agreementHandler));
    }

    startServer = async () => {
        this.api.addContentTypeParser('application/json',
            { parseAs: "string"}, // buffer?
            (req, body: string, done) => {
                try {
                    const res = JSON.parse(body);
                    done(null, res);
                } catch (e) {
                    done(null, {});
                }
            }
        );
        console.log("SERVER listening on ", apiConfig.port);
        await this.api.listen({
            port: apiConfig.port,
            host: '0.0.0.0'
        });
    }
}

export const api = new Server();