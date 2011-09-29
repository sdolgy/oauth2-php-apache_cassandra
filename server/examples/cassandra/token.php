<?php

/**
 * @file
 * Sample token endpoint.
 *
 * Obviously not production-ready code, just simple and to the point.
 *
 * In reality, you'd probably use a nifty framework to handle most of the crud for you.
 */

require "lib/CassandraOAuth2.inc";

$oauth = new CassandraOAuth2();
$oauth->grantAccessToken();
