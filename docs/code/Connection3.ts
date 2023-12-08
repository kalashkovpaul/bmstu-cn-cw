    async none(params: any) {
        if (this.state) {
            return await this.connection.none(params);
        }
        return null;
    }

    async many(params: any) {
        if (this.state) {
            return await this.connection.many(params);
        }
        return null;
    }

    async tx(params: any) {
        if (this.state) {
            return await this.connection.tx(params);
        }
        return null;
    }
}