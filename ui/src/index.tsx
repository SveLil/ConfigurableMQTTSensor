import * as React from "react";
import ReactDOM from "react-dom";
import { AppContainer } from "react-hot-loader";
// tslint:disable-next-line:no-submodule-imports
import "semantic-ui-css/semantic.min.css";
import App from "./App";

const rootEl = document.getElementById("app");
const render = (Component: typeof App) =>
  ReactDOM.render(
    <AppContainer>
      <Component />
    </AppContainer>,
    rootEl,
  );

render(App);
// Webpack Hot Module Replacement API
if ((module as any).hot) {
  (module as any).hot.accept("./App", () => {
    const NextApp = require("./App").default;
    render(NextApp);
  });
}
