import * as React from "react";
import { Form } from "semantic-ui-react";
import WifiSelector from "./WifiSelector";

export default function Wifi(): JSX.Element {
    return (
        <div>
            <p>Set up your wifi connection</p>
            <Form>
                <Form.Field label="Network" control={WifiSelector} />
                <Form.Input fluid name="wifi_password" type="password" placeholder="Password" label="Password"/>
                <Form.Checkbox label="Enable Access Point" />
                <Form.Button primary>Submit</Form.Button>
            </Form>
        </div>
    );
}
