import * as React from "react";
import { DropdownProps, Dropdown, Form } from "semantic-ui-react";
import { getNetworks, Network } from "./api";

interface WifiSelectorState {
    ssid?: string;
    networks?: Network[];
    loading: boolean;
}
export default class WifiSelector extends React.Component<{ssid?: string}, WifiSelectorState> {

    constructor(props: { ssid: string; }) {
        super(props);
        this.state = {ssid: props.ssid, loading: true};
        this.setSsid = this.setSsid.bind(this);
        this.customSSID = this.customSSID.bind(this);
        this.loadNetworks = this.loadNetworks.bind(this);
        this.loadNetworks();
    }

    customSSID(_event: React.KeyboardEvent<HTMLElement>, data: DropdownProps) {
        if (data.value) {
            const newSsid = String(data.value);
            const newNetworks = this.state.networks || [];
            if (newNetworks.some(network => network.ssid === newSsid)) {
                newNetworks.push({ssid: newSsid});
            }
            this.setState({ssid: newSsid, networks: newNetworks});
        }
    }

    loadNetworks() {
        if (!this.state.loading) {
            this.setState({ loading: true });
        }
        getNetworks().then(networks => this.setState({loading: false, networks}));
    }

    setSsid(_e: React.SyntheticEvent<HTMLElement>, data: DropdownProps): void {
        this.setState({ssid: String(data.value)});
    }

    render(): JSX.Element[] {
        const networks = this.state.networks ?
            this.state.networks.map(n => ({key: n.ssid, value: n.ssid, text: n.ssid}))
            : [];
        return [
                <Dropdown
                    selection
                    value={this.state.ssid}
                    loading={this.state.loading}
                    options={networks}
                    additionLabel=""
                    allowAdditions
                    search
                    onAddItem={this.customSSID}
                    onChange={this.setSsid}
                    text={this.state.ssid ? this.state.ssid : "Select SSID"}
                    fluid
                    key="Dropdown"
                />,
                <Form.Button key="SearchNetworks" fluid onClick={this.loadNetworks}>Search for networks</Form.Button>,
        ];
    }
}
