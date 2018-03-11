import * as React from "react";
import {
	HashRouter as Router,
	Switch,
	Route,
	Redirect,
	withRouter
} from "react-router-dom";
import AppBar from "material-ui/AppBar";
import Tabs, { Tab } from "material-ui/Tabs";
import Paper from 'material-ui/Paper';
import Reboot from "material-ui/Reboot";
import { MuiThemeProvider } from "material-ui/styles";
import { RouteComponentProps } from "react-router";
import Wifi from "./Wifi";
import Mqtt from "./Mqtt";
import Sensors from "./Sensors";
import { AppConfig, loadData } from "./api";
import { theme } from "./theme";

export const styles = (theme: any) => ({
	root: {
	  flexGrow: 1,
	},
	paper: {
	  padding: theme.spacing.unit * 2,
	  textAlign: 'center',
	  color: theme.palette.text.secondary,
	},
  });

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

function createRoutes(state: AppConfig): JSX.Element[] {
    return menuItems.map(r => (
        <Route
            path={r.route}
            key={r.route}
            render={r.render(state)}
        />
    ));
}

class NavTabs extends React.Component<RouteComponentProps<{}>,{}> {

	constructor(props: RouteComponentProps<{}>) {
		super(props);
		this.state = {value: 0};
	}

	handleChange = (_event: React.ChangeEvent<{}>, value: any) => {
		this.props.history.push(value);
	};	

	render(): JSX.Element {
		return <Tabs
				value={this.props.history.location.pathname}
				onChange={this.handleChange}
				centered
				>
				{menuItems.map(r => (
					<Tab label={r.text} value={r.route}/>
    			))}
			</Tabs>
	}
}

const NavTabsWithRouter = withRouter(NavTabs);

export default class App extends React.Component<{}, AppConfig & {loading: boolean}> {
    constructor(props: {}) {
        super(props);
        this.state = {
            loading: false,
        } as any;
        loadData().then(d => this.setState(d));
    }

    render(): JSX.Element {
        return (
			<MuiThemeProvider theme={theme}>			
				<Router>
					<div>
						<Reboot />
						<AppBar>
							<NavTabsWithRouter />
						</AppBar>
						<Paper style={{maxWidth: "600px", margin: "48px auto"}}>
							<Switch>
								{...createRoutes(this.state)}
								<Redirect from="/" to="Wifi"/>
							</Switch>
						</Paper>
					</div>
				</Router>
			</MuiThemeProvider>
        );
    }
}
