import dbConfig from '../configs/db.config.json';
import Connection from './Connection';

class ConnectManager {
    private connections: Map<string, any>;
    private connInfo: any;

    constructor() {
        this.connections = new Map();
        this.connInfo = dbConfig;
    }

    connect(connectionName: string) {
        const newConnection = new Connection(this.connInfo);
        let check = false;
        let i = 0;
        while (i < this.connInfo.repeat && !check) {
            check = newConnection.tryConnect();
            i++;
        }
        if (!check) {
            return null;
        }
        this.connections.set(connectionName, newConnection);
        return this.connections.get(connectionName);
    }
