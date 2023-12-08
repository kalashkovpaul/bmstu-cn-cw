
disconnect(connectionName: string) {
    this.connections.get(connectionName).closeConnection();
    this.connections.delete(connectionName);
}
}

export const connectManager = new ConnectManager();