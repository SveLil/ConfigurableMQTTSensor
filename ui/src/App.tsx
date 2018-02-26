import * as React from "react";
import { HashRouter as Router, NavLink, Switch, Route, Redirect } from "react-router-dom";
import { Dimmer, Menu, Container, Tab, Loader } from "semantic-ui-react";
import Wifi from "./Wifi";
import Mqtt from "./Mqtt";
import Sensors from "./Sensors";
import { AppConfig, loadData } from "./api";

interface RouteItem {
    route: string;
    text: string;
    render: (state: AppConfig) => (() => React.ReactNode);
}

const menuItems: RouteItem[] = [
    {
        text: "Wifi",
        route: "/Wifi",
        render: state => (() => <Wifi {...state.wifi} />),
    }, {
        text: "MQTT",
        route: "/MQTT",
        render: state => (() => <Mqtt {...state.mqtt} />),
    }, {
        text: "Sensors",
        route: "/Sensors",
        render: state => (() => <Sensors {...state} />),
    },
];

function createMenu(): JSX.Element[] {
    return menuItems.map(r => (
        <Menu.Item as={NavLink} to={r.route} header key={r.route}>
            {r.text}
        </Menu.Item>
    ));
}
function createRoutes(state: AppConfig): JSX.Element[] {
    return menuItems.map(r => (
        <Route
            path={r.route}
            key={r.route}
            render={r.render(state)}
        />
    ));
}
export default class App extends React.Component<{}, AppConfig & {loading: boolean}> {
    constructor(props: {}) {
        super(props);
        this.state = {
            loading: true,
        } as any;
        loadData().then(d => this.setState(d));
    }

    render(): JSX.Element {
        return (
            <Router>
                <div>
                    <Menu inverted>
                        <Container text>
                            {...createMenu()}
                        </Container>
                    </Menu>
                    <Container text>
                        <Tab.Pane style={{borderTop: "0px", marginTop: "-14px"}}>
                            <Dimmer active={this.state.loading}>
                                <Loader key="mainLoader" size="massive"/>
                            </Dimmer>
                            <Switch>
                                {...createRoutes(this.state)}
                                <Redirect from="/" to="Wifi"/>
                            </Switch>
                        </Tab.Pane>
                    </Container>
                </div>
            </Router>
        );
    }
}
