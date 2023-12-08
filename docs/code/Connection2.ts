            `${this.connectionInfo.dbName}://` +
            `${this.connectionInfo.user}:` +
            `${this.connectionInfo.password}@` +
            `${this.connectionInfo.url}:` +
            `${this.connectionInfo.port}/` +
            `${this.connectionInfo.db}`;
        try {
            this.connection = promise(connectionURL);
            this.state = true;
        } catch (e) {
            logger.error(e);
            return false;
        }
        return true;
    }

    isConnected(): boolean {
        return this.state;
    }

    closeConnection(): void {
        this.connection.$pool.end();
    }

    async one(params: any) {
        if (this.state) {
            return await this.connection.one(params);
        }
        return null;
    }