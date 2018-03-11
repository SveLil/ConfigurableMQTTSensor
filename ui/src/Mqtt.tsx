import * as React from "react";
import Button from "material-ui/Button";
import Checkbox from "material-ui/Checkbox";
import FormControlLabel from "material-ui/Form/FormControlLabel";
import TextField from "material-ui/TextField";
import withStyles from "material-ui/styles/withStyles";
import { WithStyles } from "material-ui/styles";
import { MqttConfig, saveMqtt } from "./api";
import { styles } from "./App";

export type StyledMqttConfig = MqttConfig & WithStyles<"root" | "paper">;
class Mqtt extends React.Component<StyledMqttConfig, MqttConfig> {

    constructor(props: StyledMqttConfig) {
        super(props);
        this.onChange = this.onChange.bind(this);
        this.submit = this.submit.bind(this);

        this.state = {
			baseTopic: props.baseTopic,
			enableDeepSleep: props.enableDeepSleep,
			interval: props.interval,
			mpassword: props.mpassword,
			port: props.port,
			server: props.server,
			user: props.user,
			useSSL: props.useSSL
		};
    }

    onChange(event: React.SyntheticEvent<HTMLInputElement>) {
		const data = event.target as any;
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
			<form
				style={{
					padding: '20px',
					display: 'flex',
					flexWrap: 'wrap',
				  }}
				className={this.props.classes.root}
			  >
                <h2 className={this.props.classes.paper}>Set up the connection to your MQTT server</h2>
                    <TextField
						fullWidth
                        name="server"
                        placeholder="Server"
                        label="Server"
                        defaultValue={this.props.server}
                        onChange={this.onChange}
                    />
                    <TextField
						fullWidth
                        name="port"
                        type="number"
                        placeholder="1883"
                        label="Port"
                        defaultValue={this.props.port}
                        onChange={this.onChange}
                    />
                    <FormControlLabel
						label="Use SSL"
						control={
							<Checkbox
							name="useSSL"
							defaultChecked={this.props.useSSL}
							onChange={this.onChange}
						/>}
                    />
                    <TextField
						fullWidth
                        name="user"
                        placeholder="User"
                        label="User"
                        defaultValue={this.props.user}
                        onChange={this.onChange}
                    />
                    <TextField
						fullWidth
                        name="mpassword"
                        type="password"
                        placeholder="Password"
                        label="Password"
                        defaultValue={this.props.mpassword}
                        onChange={this.onChange}
                    />
                    <TextField
						fullWidth
                        name="baseTopic"
                        placeholder="Base topic"
                        label="Base Topic"
                        defaultValue={this.props.baseTopic}
                        onChange={this.onChange}
                    />
                    <TextField
						fullWidth
						name="interval"
                        type="number"
                        placeholder="Interval"
                        label="Interval"
                        defaultValue={this.props.interval}
                        onChange={this.onChange}
                    />
                    <FormControlLabel
						label="Enable Deep Sleep"
						control={
							<Checkbox
							name="enableDeepSleep"
							defaultChecked={this.props.enableDeepSleep}
							onChange={this.onChange}
						/>}
                    />
                    <Button fullWidth variant="raised" color="primary" onClick={this.submit}>Submit</Button>
                </form>
        );
    }
}

export default withStyles(styles)<MqttConfig>(Mqtt);
