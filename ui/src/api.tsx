function fetchAsJson(url: string): Promise<any> {
    return fetch(url).then(r => {
        if (r.ok) {
            return r.json();
        } else {
            throw r;
        }
    });
}

function post(url: string, data: any): Promise<any> {
    let params = "";
    for (const key of Object.keys(data)) {
        if (params !== "") {
            params += "&";
        }
        params += key + "=" + encodeURIComponent(data[key]);
    }
    const postUrl = url + params;
    return fetch(postUrl, {
        method: "POST",
        body: JSON.stringify(data),
        headers: {
            "content-type": "application/json",
        },
    });
}

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
    return fetchAsJson("sensorTypeInfo");
}

export interface Network {
    ssid: string;
}

export function getNetworks(): Promise<Network[]> {
    return fetchAsJson("scan").then(r => r.map((ssid: string) => ({ssid})));
}

export interface MqttConfig {
    server: string;
    port: number;
    useSSL: boolean;
    user: string;
    mpassword: string;
    baseTopic: string;
    interval: number;
    enableDeepSleep: boolean;
}

export function saveMqtt(config: MqttConfig) {
    post("saveMQTT", config);
}

export interface WifiConfig {
    ssid: string;
    wpassword: string;
    enableAP: boolean;
}

export function saveWifi(config: WifiConfig) {
    post("saveWiFi", config);
}

export interface EspConfig {
    ssid: string;
    wpassword: string;
    server: string;
    port: number;
    user: string;
    mpassword: string;
    useSSL: boolean;
    interval: number;
    baseTopic: string;
    enableAP: boolean;
    enableDeepSleep: boolean;
    sensors: Sensor[];
}

export interface AppConfig {
    wifi: WifiConfig;
    sensors: Sensor[];
    mqtt: MqttConfig;
 }

export function loadData(): Promise<AppConfig> {
    return fetchAsJson("load").then(v => {
        const c = v as EspConfig;
        const wifi: WifiConfig = {
            ssid: c.ssid,
            wpassword: c.wpassword,
            enableAP: c.enableAP,
        };
        const mqtt: MqttConfig = {
            server: c.server,
            port: c.port,
            useSSL: c.useSSL,
            user: c.user,
            mpassword: c.mpassword,
            baseTopic: c.baseTopic,
            interval: c.interval,
            enableDeepSleep: c.enableDeepSleep,
        };
        return {
            wifi,
            mqtt,
            sensors: c.sensors,
        };
    });
}
