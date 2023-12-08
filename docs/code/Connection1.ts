import logger from '../logger';
import pgPromise from 'pg-promise';

export default class Connection {
    private state: boolean;
    private connectionInfo: any;
    private connection: any;


    constructor(connInfo: any) {
        this.connectionInfo = connInfo;
        this.state = false;
    }

    tryConnect(): boolean {
        const promise = pgPromise();
        const connectionURL =