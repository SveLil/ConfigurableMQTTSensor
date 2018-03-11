import * as React from "react";
import withStyles from "material-ui/styles/withStyles";
import { WithStyles } from "material-ui/styles";
import SensorForm from "./SensorForm";
import { styles } from "./App";
import { Sensor, SensorConfig, getSensorConfig } from "./api";

export type StyledSensorsConfig = { sensors: Sensor[]} & WithStyles<"root" | "paper">;
class Sensors extends React.Component<StyledSensorsConfig,
    {
        sensors: Sensor[];
        sensorConfig?: SensorConfig;
    }> {

    constructor(props: StyledSensorsConfig) {
        super(props);
        this.onAddSensor = this.onAddSensor.bind(this);
        this.onRemoveSensor = this.onRemoveSensor.bind(this);
        getSensorConfig().then(sc => {
            this.setState({sensorConfig: sc});
        });
        this.state = {
            sensors: props.sensors,
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
            ] : [<div> Loading </div>];
        return (
			<div
				style={{
					padding: '20px',
					display: 'flex',
					flexWrap: 'wrap',
			 	}}
				 className={this.props.classes.root}
			>
                <h2 className={this.props.classes.paper}>Set up your sensors</h2>
                {...elements}
            </div>
        );
    }
}

export default withStyles(styles)<{ sensors: Sensor[]}>(Sensors);
