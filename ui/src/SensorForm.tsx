import * as React from "react";
import { Form, DropdownProps, Segment } from "semantic-ui-react";
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
                return <Form.Checkbox
                    label={param.name}
                    key={param.name}
                    name={param.name}
                    value={this.state ? this.state.currentConfig[param.name] : param.value || ""}
                    onChange={this.onChange as any}
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
        return <Form.Input
            label={param.name}
            key={param.name}
            name={param.name}
            type={type}
            required
            value={this.state ? this.state.currentConfig[param.name] : param.value || ""}
            onChange={this.onChange as any}
        />;
    }

    onChange(_event: React.SyntheticEvent<HTMLInputElement>, data: {name: string, value: string}) {
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
                type: "",
                currentConfig: {
                        name: "",
                    },
                formElements: [<p key="info">Please select a sensor type</p>],
                changed: false,
            });
        }
    }

    onSelectSensor(_event: React.SyntheticEvent<HTMLElement>, data: DropdownProps) {
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
            <Segment>
                <Form onSubmit={this.onSubmit}>
                 {
                    this.props.sensor ?
                    <Form.Input readOnly label="Type" value={this.props.sensor.sensorType} /> :
                    <Form.Dropdown
                        label="Type"
                        name="type"
                        selection
                        options={keys.map(k => ({key: k, value: k, text: k}))}
                        onChange={this.onSelectSensor}
                    />
                 }
                    <Form.Input
                        label="Name"
                        value={this.state.currentConfig.name}
                        name="name"
                        required
                        onChange={this.onChange as any}
                    />
                    {...this.state.formElements}
                    <Form.Group>
                        <Form.Button primary disabled={!this.state.changed}>
                            {this.props.sensor ? "Update Sensor" : "Add Sensor"}
                        </Form.Button>
                        {this.props.sensor ?
                            <Form.Button negative onClick={this.props.onRemove as any}>Remove Sensor</Form.Button> :
                            ""
                        }
                    </Form.Group>
                </Form>
            </Segment>
        );
    }
}
