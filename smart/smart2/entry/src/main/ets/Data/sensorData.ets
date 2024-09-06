// DataStore.ts
class DataStore {
  private static instance: DataStore;
  private store: { [key: string]: any } = {};

  private constructor() {}

  public static getInstance(): DataStore {
    if (!DataStore.instance) {
      DataStore.instance = new DataStore();
    }
    return DataStore.instance;
  }

  public setItem(key: string, value: any) {
    this.store[key] = value;
  }

  public getItem(key: string): any {
    return this.store[key];
  }
}

export default DataStore.getInstance();
