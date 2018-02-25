import * as React from "react";
import {Divider, Form } from "semantic-ui-react";

export default function Mqtt(): JSX.Element {
    return (
        <div>
            <p>Set up the connection to your MQTT server</p>
            <Form>
                <Form.Input fluid name="server" placeholder="Server" label="Server"/>
                <Form.Input fluid name="port" type="number" placeholder="1883" label="Port"/>
                <Form.Checkbox label="Use SSL" />
                <Divider/>
                <Form.Input fluid name="user" placeholder="User" label="User"/>
                <Form.Input fluid name="wifi_password" type="password" placeholder="Password" label="Password"/>
                <Divider/>
                <Form.Input fluid name="baseTopic" placeholder="Base topic" label="Base Topic"/>
                <Form.Input fluid name="interval" type="number" placeholder="Interval" label="Interval"/>
                <Form.Checkbox label="Enable Deep Sleep" />
                <Form.Button primary>Submit</Form.Button>
            </Form>
        </div>
    );
}
