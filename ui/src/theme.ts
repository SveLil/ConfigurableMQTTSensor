import { createMuiTheme } from 'material-ui/styles';
import blue from 'material-ui/colors/blue';
import pink from 'material-ui/colors/pink';
import red from 'material-ui/colors/red';

// All the following keys are optional.
// We try our best to provide a great default value.
export const theme = createMuiTheme({
  palette: {
    primary: blue,
    secondary: pink,
    error: red,
  },
});