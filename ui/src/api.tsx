export interface SensorParameterConfig {
    configType: "STRING" | "INTEGER" | "FLOAT" | "BOOLEAN";
    name: string;
}

export interface SensorConfig {
    [key: string]: SensorParameterConfig[];
}

export interface SensorParameter {
    name: string;
    value: string | boolean | number;
}

export interface Sensor {
    sensorId: number;
    sensorName: string;
    sensorType: string;
    config: SensorParameter[];
}

export function getSensorConfig(): Promise<SensorConfig> {
    const sensorConfig: SensorConfig = {
        SHT22: [ {configType: "INTEGER", name: "Pin"} ],
    };
    return Promise.resolve(sensorConfig);
}
