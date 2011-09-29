<?php

// Set these values to your Cassandra database

define("PHPCASSA_VERSION", '0.8.a.2');
define("CASSANDRA_SERVER", "127.0.0.1:9160");
define("CASSANDRA_KEYSPACE", "Keyspace1");
define("CASSANDRA_CF", "oauth");

require_once dirname(__file__) . '/phpcassa-' . PHPCASSA_VERSION . '/connection.php';
require_once dirname(__file__) . '/phpcassa-' . PHPCASSA_VERSION . '/columnfamily.php';
require_once dirname(__file__) . '/../../../lib/oauth.php';

/**
 * Sample OAuth2 Library Apache Cassandra Implementation.
 */
class CassandraOAuth2 extends OAuth2 {

	private $db;

	/**
	 * Overrides OAuth2::__construct().
	 */
	public function __construct() {
		parent::__construct();

		$pool = new ConnectionPool(CASSANDRA_KEYSPACE, array(CASSANDRA_SERVER));
		$this->db = new ColumnFamily($pool, CASSANDRA_CF);
	}

	/**
	 * Little helper function to add a new client to the database.
	 *
	 * Do NOT use this in production! This sample code stores the secret
	 * in plaintext!
	 *
	 * @param $client_id
	 *   Client identifier to be stored.
	 * @param $client_secret
	 *   Client secret to be stored.
	 * @param $redirect_uri
	 *   Redirect URI to be stored.
	 */
	public function addClient($client_id, $client_secret, $redirect_uri) {
		$this->db->set('clients', array($client_id => json_encode(array('client_secret' => $client_secret, 'redirect_uri' => $redirect_uri))));
	}

	/**
	 * Implements OAuth2::checkClientCredentials().
	 *
	 * Do NOT use this in production! This sample code stores the secret
	 * in plaintext!
	 */
	protected function checkClientCredentials($client_id, $client_secret = NULL) {
		try {
			$client = $this->db->get('clients', array($client_id));
			$client_info = json_decode($client[$client_id], true);
			if (isset($client_info['client_secret'])) {
				if ($client_secret == $client_info['client_secret']) {
					return $client;
				} else {
					return FALSE;
				}
			} else {
				return FALSE;
			}
		} catch (Exception $e) {
			return FALSE;
		}
	}

	/**
	 * Implements OAuth2::getRedirectUri().
	 */
	protected function getRedirectUri($client_id) {
		try {
			$client = $this->db->get('clients', array($client_id));
			$client_info = json_decode($client[$client_id], true);
			if (isset($client_info['redirect_uri'])) {
				return $client_info['redirect_uri'];
			} else {
				return FALSE;
			}
		} catch (Exception $e) {
			return FALSE;
		}
	}

	/**
	 * Implements OAuth2::getAccessToken().
	 */
	protected function getAccessToken($oauth_token) {
		try {
			return $this->db->get('tokens', array($oauth_token));
		} catch (Exception $e) {
			return FALSE;
		}		
	}

	/**
	 * Implements OAuth2::setAccessToken().
	 */
	protected function setAccessToken($oauth_token, $client_id, $expires, $scope = NULL) {
		try {
			$client = $this->db->set('tokens', array($oauth_token => json_encode(array('client_id' => $client_id, 'expires' => $expires, 'scope' => $scope))));
		} catch (Exception $e) {
			return FALSE;
		}		
	}

	/**
	 * Overrides OAuth2::getSupportedGrantTypes().
	 */
	protected function getSupportedGrantTypes() {
		return array(
		OAUTH2_GRANT_TYPE_AUTH_CODE,
		);
	}

	/**
	 * Overrides OAuth2::getAuthCode().
	 */
	protected function getAuthCode($code) {
		try { 
			return $this->db->get('auth_codes', array('$code'));
		} catch (Exception $e) { 
			return FALSE;
		}
	}

	/**
	 * Overrides OAuth2::setAuthCode().
	 */
	protected function setAuthCode($code, $client_id, $redirect_uri, $expires, $scope = NULL) {
		try { 
			$this->db->set('auth_codes', array($code => json_encode(array('client_id' => $client_id, 'redirect_uri' => $redirect_uri, 'expires' => $expires, 'scope' => $scope))));
		} catch (Exception $e) {
			return FALSE;
		}
	}
}
