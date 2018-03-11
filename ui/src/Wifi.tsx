import * as React from "react";
import Button from "material-ui/Button";
import Checkbox from "material-ui/Checkbox";
import FormControlLabel from "material-ui/Form/FormControlLabel";
import TextField from "material-ui/TextField";
import withStyles from "material-ui/styles/withStyles";
import { WithStyles } from "material-ui/styles";
import { WifiConfig, saveWifi } from "./api";
import WifiSelector from "./WifiSelector";
import { styles } from "./App";

export type StyledWifiConfig = WifiConfig & WithStyles<"root" | "paper">;
class Wifi extends React.Component<StyledWifiConfig, WifiConfig> {

    constructor(props: StyledWifiConfig) {
        super(props);
        this.onChange = this.onChange.bind(this);
        this.submit = this.submit.bind(this);
        this.state = {
			ssid: props.ssid,
			wpassword: props.wpassword,
			enableAP: props.enableAP,
		};
    }

    onChange(event: React.ChangeEvent<HTMLElement>) {
		const data = event.target as any;
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
			<form
				style={{
					padding: '20px',
					display: 'flex',
					flexWrap: 'wrap',
				  }}
				className={this.props.classes.root}
			  >
				<h2 className={this.props.classes.paper}>Set up your wifi connection</h2>
				<WifiSelector
					ssid={this.props.ssid}
					onChange={this.onChange}
				/>
				<TextField
					fullWidth
					name="wpassword"
					type="password"
					placeholder="Password"
					label="Password"
					defaultValue={this.props.wpassword}
					onChange={this.onChange}
				/>
				<FormControlLabel
					label="Enable Access Point"
					control={
						<Checkbox
							name="enableAP"
							defaultChecked={this.props.enableAP}
							onChange={this.onChange}
						/>
					}
					/>
				<Button fullWidth variant="raised" color="primary" onClick={this.submit}>Submit</Button>
			</form>
        );
    }
}

export default withStyles(styles)<WifiConfig>(Wifi);
