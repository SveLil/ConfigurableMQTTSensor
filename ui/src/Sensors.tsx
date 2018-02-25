import * as React from "react";
import SensorForm from "./SensorForm";
import { Sensor, SensorConfig, getSensorConfig } from "./api";
import { Loader } from "semantic-ui-react";

export interface SensorsState {
    sensors: Sensor[];
    sensorConfig?: SensorConfig;
}

export default class Sensors extends React.Component<{}, SensorsState> {

    constructor(props: {}) {
        super(props);
        this.onAddSensor = this.onAddSensor.bind(this);
        this.onRemoveSensor = this.onRemoveSensor.bind(this);
        getSensorConfig().then(sc => {
            this.setState({sensorConfig: sc});
        });
        this.state = {
            sensors: [],
        };
    }

    onAddSensor(newSensor: Sensor) {
        const newSensors = [...this.state.sensors, newSensor];
        this.setState({sensors: newSensors});
    }

    onRemoveSensor(oldSensor: Sensor) {
        const newSensors = this.state.sensors.filter(s => s.sensorId !== oldSensor.sensorId);
        this.setState({sensors: newSensors});
    }

    onUpdateSensor(newSensor: Sensor) {
        const newSensors = this.state.sensors.map(s => s.sensorId !== newSensor.sensorId ? s : newSensor);
        this.setState({sensors: newSensors});
    }

    render(): JSX.Element {
        const sensorConfig = this.state.sensorConfig;
        const elements: JSX.Element[] = sensorConfig ?
            [
            <SensorForm
                    key="-1"
                    sensorConfig={sensorConfig}
                    onSubmit={this.onAddSensor}
            />,
            ...this.state.sensors.map(s =>
                <SensorForm
                    key={s.sensorId}
                    sensor={s}
                    sensorConfig={sensorConfig}
                    onSubmit={this.onUpdateSensor}
                    onRemove={this.onRemoveSensor}
                />),
            ] : [ <Loader /> ];
        return (
            <div>
                <p>Set up your sensors</p>
                {...elements}
            </div>
        );
    }
}
