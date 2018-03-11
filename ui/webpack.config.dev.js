var path = require('path');
var webpack = require('webpack');
const ForkTsCheckerWebpackPlugin = require('fork-ts-checker-webpack-plugin');
const ExtractTextPlugin = require('extract-text-webpack-plugin');
console.log(process.env.NODE_ENV === 'production' ? "Production build" : "Development build");
module.exports = {
  entry: [
    'react-hot-loader/patch',
    './src/index.tsx',
  ],

  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'bundle.js',
    publicPath: '/'
  },
  devtool: 'cheap-module-eval-source-map',
  module: {
    rules: [
      {
        test: /.tsx?$/,
        loaders: ['react-hot-loader/webpack', 'ts-loader'],
        exclude: /node_modules/
      },
      {
        test: /.css$/,
        use: [ 'style-loader', 'css-loader' ]
      },
      {
        test: /\.(png|jpg|gif|svg|eot|ttf|woff|woff2)$/,
        loader: 'url-loader',
        options: {
          limit: 10000
        }
      }
    ]
  },
  plugins: [
    new webpack.HotModuleReplacementPlugin(),
    new webpack.NamedModulesPlugin(),
    new webpack.NoEmitOnErrorsPlugin()
  ],
  devServer: {
    host: 'localhost',
    port: 3000,
    historyApiFallback: true,
    hot: true
  },
  resolve: {
    extensions: ['.ts', '.tsx', '.js']
  },

}