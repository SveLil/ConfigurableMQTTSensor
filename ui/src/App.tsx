import * as React from "react";
import { HashRouter as Router, NavLink, Switch, Route, Redirect } from "react-router-dom";
import { Menu, Container, Tab } from "semantic-ui-react";
import Wifi from "./Wifi";
import Mqtt from "./Mqtt";
import Sensors from "./Sensors";

interface RouteItem {
    component: React.ComponentType;
    route: string;
    text: string;
}

const menuItems: RouteItem[] = [
    {
        text: "Wifi",
        route: "/Wifi",
        component: Wifi,
    }, {
        text: "MQTT",
        route: "/MQTT",
        component: Mqtt,
    }, {
        text: "Sensors",
        route: "/Sensors",
        component: Sensors,
    },
];

function createMenu(): JSX.Element[] {
    return menuItems.map(r => (
        <Menu.Item as={NavLink} to={r.route} header key={r.route}>
            {r.text}
        </Menu.Item>
    ));
}
function createRoutes(): JSX.Element[] {
    return menuItems.map(r => (
        <Route path={r.route} component={r.component} key={r.route} />
    ));
}
export default function App(): JSX.Element {
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
                    <Switch>
                        {...createRoutes()}
                        <Redirect from="/" to="Wifi"/>
                    </Switch>
                </Tab.Pane>
                </Container>
            </div>
        </Router>
    );
}
