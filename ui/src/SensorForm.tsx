import * as React from "react";
import Button from "material-ui/Button";
import Checkbox from "material-ui/Checkbox";
import FormControlLabel from "material-ui/Form/FormControlLabel";
import TextField from "material-ui/TextField";
import Paper from "material-ui/Paper";
import Select from 'material-ui/Select';
import { InputLabel } from 'material-ui/Input';
import { FormControl } from 'material-ui/Form';
import { MenuItem } from 'material-ui/Menu';

import { Sensor, SensorConfig, SensorParameterConfig } from "./api";

export interface SensorFormProps {
    sensor?: Sensor;
    sensorConfig: SensorConfig;
    onSubmit: (newSensor: Sensor) => void;
    onRemove?: (oldSensor: Sensor) => void;
}

export interface SensorFormState {
    type: string;
    currentConfig: {[key: string]: any};
    formElements: JSX.Element[];
    changed: boolean;
}

export default class SensorForm extends React.Component<SensorFormProps, SensorFormState> {

    constructor(props: SensorFormProps) {
        super(props);
        this.onChange = this.onChange.bind(this);
        this.onSelectSensor = this.onSelectSensor.bind(this);
        this.onSubmit = this.onSubmit.bind(this);
        this.inputTypeForConfigType = this.inputTypeForConfigType.bind(this);
        const initialConfig: {[key: string]: any} = {};
        let initalFormElements: JSX.Element[];
        if (props.sensor) {
            props.sensor.config.forEach(v => initialConfig[v.name] = v.value);
            initialConfig.name = props.sensor.sensorName;
            initalFormElements = props.sensorConfig[props.sensor.sensorType]
                    .map(v => ({...v, value: initialConfig[v.name]}))
                    .map(this.inputTypeForConfigType);
        } else {
            initialConfig.name = "";
            initalFormElements = [<p key="info">Please select a sensor type</p>];
        }
        this.state = {
            type: "",
            currentConfig: initialConfig,
            formElements: initalFormElements,
            changed: false,
        };
    }

    inputTypeForConfigType(param: SensorParameterConfig & {value: string}): JSX.Element {
        let type: string;
        switch (param.configType) {
            case "BOOLEAN":
                return <FormControlLabel
				label={param.name}
				control={
					<Checkbox
	                    key={param.name}
						name={param.name}
						value={this.state ? this.state.currentConfig[param.name] : param.value || ""}
						onChange={this.onChange as any}
					/>
				}
                />;
            case "INTEGER":
                type = "number";
                break;
            case "FLOAT":
                type = "number";
                break;
            case "STRING":
                type = "text";
                break;
            default:
                type = "text";
                break;
        }
        return <TextField
            label={param.name}
            key={param.name}
            name={param.name}
            type={type}
            required
            value={this.state ? this.state.currentConfig[param.name] : param.value || ""}
            onChange={this.onChange as any}
        />;
    }

    onChange(event: React.SyntheticEvent<HTMLInputElement>) {
		const data = event.target as any;
        const newSensorConfg: any = {...this.state.currentConfig};
        newSensorConfg[data.name] = data.value;
        this.setState({currentConfig: newSensorConfg, changed: true});
    }

    onSubmit() {
        const newConfig = Object.keys(this.state.currentConfig)
            .filter(k => k !== "name")
            .map(k => {
                return {name: k, value: this.state.currentConfig[k]};
            });
        const newSensor: Sensor = {
            sensorType: this.state.type,
            sensorId: this.props.sensor ? this.props.sensor.sensorId : -1,
            sensorName: this.state.currentConfig.name,
            config: newConfig,
        };
        this.props.onSubmit(newSensor);
        if (!this.props.sensor) {
            // New sensor form
            this.setState({
                type: " ",
                currentConfig: {
                        name: "",
                    },
                formElements: [<p key="info">Please select a sensor type</p>],
                changed: false,
            });
        }
    }

    onSelectSensor(event: React.SyntheticEvent<HTMLElement>) {
		const data = event.target as any;
        const sensor = data.value ? String(data.value) : undefined;
        if (sensor) {
            const currentConfig = this.state.currentConfig;
            this.setState(
                {
                    type: sensor,
                    formElements: this.props.sensorConfig[sensor]
                        .map(v => ({...v, value: currentConfig[v.name]}))
                        .map(this.inputTypeForConfigType),
                },
            );
        }
    }

    render(): JSX.Element {
        const keys = Object.keys(this.props.sensorConfig);
        return (
			<Paper>
                <form onSubmit={this.onSubmit}>
                 {
                    this.props.sensor ?
					<TextField disabled label="Type" value={this.props.sensor.sensorType} /> :
					<FormControl>
						<InputLabel htmlFor="type-select">Type</InputLabel>
						<Select
							inputProps={{
								name: "type",
								id: "type-select",
							}}
							value={this.state.type}
							onChange={this.onSelectSensor}
						>
						{keys.map(k => <MenuItem value={k}>{k}</MenuItem>)}
						</Select>
					</FormControl>
                 }
                    <TextField
                        label="Name"
                        value={this.state.currentConfig.name}
                        name="name"
                        required
                        onChange={this.onChange as any}
                    />
                    {...this.state.formElements}
                    <div>
                        <Button variant="raised" color="primary" disabled={!this.state.changed}>
                            {this.props.sensor ? "Update Sensor" : "Add Sensor"}
                        </Button>
                        {this.props.sensor ?
                            <Button variant="raised" color="primary" onClick={this.props.onRemove as any}>Remove Sensor</Button> :
                            ""
                        }
                    </div>
                </form>
			</Paper>
        );
    }
}
