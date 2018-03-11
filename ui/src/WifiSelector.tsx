import * as React from "react";
import { getNetworks, Network } from "./api";
import Select from 'material-ui/Select';
import Button from "material-ui/Button";
import { MenuItem } from "material-ui/Menu";
import { InputLabel } from 'material-ui/Input';
import Grid from "material-ui/Grid"
import { FormControl } from "material-ui";
import withStyles from "material-ui/styles/withStyles";
import { WithStyles } from "material-ui/styles";

interface WifiSelectorState {
    ssid?: string;
    networks?: Network[];
    loading: boolean;
}

interface WifiSelectorProps {
	ssid?: string;
	onChange: (event: React.ChangeEvent<HTMLInputElement | HTMLTextAreaElement>) => void;
}

const styles = (theme: any) => ({
	root: {
	  flexGrow: 1,
	},
	paper: {
	  padding: theme.spacing.unit * 2,
	  textAlign: 'center',
	  color: theme.palette.text.secondary,
	},
  });

export type StyledWifiSelectorProps = WifiSelectorProps & WithStyles<"root" | "paper">;

class WifiSelector extends React.Component<StyledWifiSelectorProps, WifiSelectorState> {

    constructor(props: StyledWifiSelectorProps) {
        super(props);
        this.state = {ssid: props.ssid, loading: true};
        this.loadNetworks();
    }

    customSSID = (event: React.KeyboardEvent<HTMLElement>) => {
		const data = event.target as any;
        if (data.value) {
            const newSsid = String(data.value);
            const newNetworks = this.state.networks || [];
            if (newNetworks.some(network => network.ssid === newSsid)) {
                newNetworks.push({ssid: newSsid});
            }
            this.setState({ssid: newSsid, networks: newNetworks});
        }
    }

    loadNetworks = () => {
        if (!this.state.loading) {
            this.setState({ loading: true });
        }
        getNetworks().then(networks => this.setState({loading: false, networks}));
    }

    setSsid = (event: React.ChangeEvent<HTMLInputElement | HTMLTextAreaElement>) => {
		const data = event.target as any;
		this.setState({ssid: String(data.value)});
		this.props.onChange ? this.props.onChange(event) : "";
    }

    render(): JSX.Element | JSX.Element[] {
        const networks = this.state.networks ?
            this.state.networks.map(n => (<MenuItem key={n.ssid}>{n.ssid}</MenuItem>))
            : [];
        return [
				<Grid item xs={7}>
					<FormControl fullWidth>
						<InputLabel htmlFor="ssid-input">Network</InputLabel>
						<Select
							fullWidth
							value={this.state.ssid}
							onChange={this.setSsid}
							name="ssid"
							native
							key="Dropdown"
							inputProps={{
								name: "ssid",
								id: "ssid-input",
							}}
						>
							{...networks}
						</Select>
					</FormControl>
				</Grid>,
				<Grid item xs={5}>
                	<Button key="SearchNetworks" variant="raised" color="primary" onClick={this.loadNetworks}>Search for networks</Button>
				</Grid>,
		];
    }
}

export default withStyles(styles)<WifiSelectorProps>(WifiSelector);