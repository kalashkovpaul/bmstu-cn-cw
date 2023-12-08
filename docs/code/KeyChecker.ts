class KeyChecker {
    private data: Map<string, number>;

    constructor() {
        this.data = new Map();
    }

    registerKey(key: string, owner: number): boolean {
        if (this.data.get(key)) {
            return false;
        }
        this.data.set(key, owner);
        return true;
    }

    removeKey(key: string) {
        if (this.data.has(key)) {
            this.data.delete(key);
        }
    }

    getOwner(key: string): number {
        if (this.data.has(key)) {
            return this.data.get(key) as number;
        }
        return -1;
    }
}

export const keyChecker = new KeyChecker();