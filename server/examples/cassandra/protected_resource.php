<?php

/**
 * @file
 * Sample protected resource.
 *
 * Obviously not production-ready code, just simple and to the point.
 *
 * In reality, you'd probably use a nifty framework to handle most of the crud for you.
 */

require "lib/CassandraOAuth2.inc";

$oauth = new CassandraOAuth2();
$oauth->verifyAccessToken();

// With a particular scope, you'd do:
// $oauth->verifyAccessToken("scope_name");

?>

<html>
  <head>
    <title>Hello!</title>
  </head>
  <body>
    <p>This is a secret.</p>
  </body>
</html>
