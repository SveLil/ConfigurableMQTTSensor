const path = require('path');
const webpack = require('webpack');
const BabiliPlugin = require("babili-webpack-plugin");
const ExtractTextPlugin = require('extract-text-webpack-plugin');
const BundleAnalyzerPlugin = require('webpack-bundle-analyzer').BundleAnalyzerPlugin;

console.log(process.env.NODE_ENV === 'production' ? "Production build" : "Development build");
module.exports = {
  entry: [
    './src/index.tsx',
  ],

  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'bundle.js'
  },
  devtool: false,
  module: {
    rules: [
      {
        test: /.tsx?$/,
        loaders: [
			'ts-loader'
		],
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
	new ExtractTextPlugin({
		filename: '[name].css'
	}),
	new BundleAnalyzerPlugin(),
	new BabiliPlugin(),
  ],
  resolve: {
    modules: [
      'node_modules',
      path.resolve(__dirname, 'app')
    ],
    extensions: ['.tsx','.ts', '.js']
  },

}