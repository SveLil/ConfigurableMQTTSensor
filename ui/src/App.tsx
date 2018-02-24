import * as React from "react";
import { HashRouter as Router, NavLink, Switch, Route, Redirect } from "react-router-dom";
import { Menu, Container } from "semantic-ui-react";
import Wifi from "./Wifi";

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
        component: Wifi,
    }, {
        text: "Sensors",
        route: "/Sensors",
        component: Wifi,
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
                <Menu fixed="top" inverted>
                    <Container>
                        {...createMenu()}
                    </Container>
                </Menu>
                <Switch>
                    {...createRoutes()}
                    <Redirect from="/" to="Wifi"/>
                </Switch>
            </div>
        </Router>
    );
}
