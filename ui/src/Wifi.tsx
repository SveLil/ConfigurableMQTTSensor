import * as React from "react";
import { Form, InputOnChangeData, CheckboxProps } from "semantic-ui-react";
import WifiSelector from "./WifiSelector";
import { WifiConfig, saveWifi } from "./api";

export default class Mqtt extends React.Component<WifiConfig, WifiConfig> {

    constructor(props: WifiConfig) {
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
        saveWifi(this.state);
    }

    render(): JSX.Element {
        return (
            <div>
                <p>Set up your wifi connection</p>
                <Form>
                    <Form.Field
                        label="Network"
                        name="ssid"
                        control={WifiSelector}
                        defaultValue={this.props.ssid}
                        onChange={this.onChange}
                    />
                    <Form.Input
                        fluid
                        name="wpassword"
                        type="password"
                        placeholder="Password"
                        label="Password"
                        defaultValue={this.props.wpassword}
                        onChange={this.onChange}
                    />
                    <Form.Checkbox
                        label="Enable Access Point"
                        name="enableAP"
                        defaultChecked={this.props.enableAP}
                        onChange={this.onChange}
                    />
                    <Form.Button primary onClick={this.submit}>Submit</Form.Button>
                </Form>
            </div>
        );
    }
}
