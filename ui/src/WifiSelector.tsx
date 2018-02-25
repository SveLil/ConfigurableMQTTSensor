import * as React from "react";
import { DropdownProps, Dropdown, Form } from "semantic-ui-react";

interface WifiSelectorState {
    ssid?: string;
    networks?: string[];
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
            if (newNetworks.indexOf(newSsid) < 0) {
                newNetworks.push(newSsid);
            }
            this.setState({ssid: newSsid, networks: newNetworks});
        }
    }

    loadNetworks() {
        if (!this.state.loading) {
            this.setState({ loading: true });
        }
        // tslint:disable-next-line:no-this-assignment
        const that = this;
        setTimeout(
            () => that.setState({loading: false, networks: ["H2G2", "Test", "3rd one"]}), 1000);
    }

    setSsid(_e: React.SyntheticEvent<HTMLElement>, data: DropdownProps): void {
        this.setState({ssid: String(data.value)});
    }

    render(): JSX.Element[] {
        const networks = this.state.networks ?
            this.state.networks.map(n => ({key: n, value: n, text: n}))
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
                />,
                <Form.Button fluid onClick={this.loadNetworks}>Search for networks</Form.Button>,
        ];
    }
}
