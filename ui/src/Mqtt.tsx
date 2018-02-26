import * as React from "react";
import {Divider, Form, InputOnChangeData, CheckboxProps } from "semantic-ui-react";
import { MqttConfig, saveMqtt } from "./api";

export default class Mqtt extends React.Component<MqttConfig, MqttConfig> {

    constructor(props: MqttConfig) {
        super(props);
        this.onChange = this.onChange.bind(this);
        this.submit = this.submit.bind(this);

        this.state = {...props};
    }

    onChange(_event: React.SyntheticEvent<HTMLInputElement>, data: InputOnChangeData | CheckboxProps) {
        this.setState(prevState => {
            const newState: any = {...prevState};
            newState[data.name] = data.checked !== undefined ? data.checked : data.value;
            return newState;
        });
    }

    submit() {
        saveMqtt(this.state);
    }

    render(): JSX.Element {
        return (
            <div>
                <p>Set up the connection to your MQTT server</p>
                <Form>
                    <Form.Input
                        fluid
                        name="server"
                        placeholder="Server"
                        label="Server"
                        defaultValue={this.props.server}
                        onChange={this.onChange}
                    />
                    <Form.Input
                        fluid
                        name="port"
                        type="number"
                        placeholder="1883"
                        label="Port"
                        defaultValue={this.props.port}
                        onChange={this.onChange}
                    />
                    <Form.Checkbox
                        label="Use SSL"
                        name="useSSL"
                        defaultChecked={this.props.useSSL}
                        onChange={this.onChange}
                    />
                    <Divider/>
                    <Form.Input
                        fluid
                        name="user"
                        placeholder="User"
                        label="User"
                        defaultValue={this.props.user}
                        onChange={this.onChange}
                    />
                    <Form.Input
                        fluid
                        name="mpassword"
                        type="password"
                        placeholder="Password"
                        label="Password"
                        defaultValue={this.props.mpassword}
                        onChange={this.onChange}
                    />
                    <Divider/>
                    <Form.Input
                        fluid
                        name="baseTopic"
                        placeholder="Base topic"
                        label="Base Topic"
                        defaultValue={this.props.baseTopic}
                        onChange={this.onChange}
                    />
                    <Form.Input
                        fluid
                        name="interval"
                        type="number"
                        placeholder="Interval"
                        label="Interval"
                        defaultValue={this.props.interval}
                        onChange={this.onChange}
                    />
                    <Form.Checkbox
                        label="Enable Deep Sleep"
                        name="enableDeepSleep"
                        defaultChecked={this.props.enableDeepSleep}
                        onChange={this.onChange}
                    />
                    <Form.Button primary onClick={this.submit}>Submit</Form.Button>
                </Form>
            </div>
        );
    }
}
