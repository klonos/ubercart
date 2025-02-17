<?php
/**
 * @file
 * Contains Order classes.
 */

/**
 * Defines an order object.
 */
class UcOrder extends Entity {

  public $order_id = 0;
  public $uid = 0;
  public $order_status = '';
  public $order_total = 0;
  public $primary_email = '';

  public $delivery_first_name = '';
  public $delivery_last_name = '';
  public $delivery_phone = '';
  public $delivery_company = '';
  public $delivery_street1 = '';
  public $delivery_street2 = '';
  public $delivery_city = '';
  public $delivery_zone = 0;
  public $delivery_postal_code = '';
  public $delivery_country = 0;

  public $billing_first_name = '';
  public $billing_last_name = '';
  public $billing_phone = '';
  public $billing_company = '';
  public $billing_street1 = '';
  public $billing_street2 = '';
  public $billing_city = '';
  public $billing_zone = 0;
  public $billing_postal_code = '';
  public $billing_country = 0;

  public $products = array();
  public $line_items = array();

  public $payment_method = '';
  public $data = array();
  public $created = 0;
  public $modified = 0;
  public $currency = '';

  /**
   * Order object constructor.
   *
   * @param $uid
   *   The user ID that owns the order, or a cart ID. Cart IDs are integer
   *   user IDs for authenticated users, or are strings of 22 characters
   *   or more for anonymous users.
   * @param $state
   *   The initial order state.
   */
  // function __construct($uid = 0, $state = 'in_checkout') {
  function __construct(array $values) {
    $uid = isset($values['uid']) ? $values['uid'] : 0;
    $state = isset($values['state']) ? $values['state'] : 'in_checkout';
    if (strlen($uid) < 22 && $uid > 0) {
      $this->uid = $uid;
      if ($account = user_load($uid)) {
        $this->primary_email = $account->mail;
      }
    }

    $this->order_status = uc_order_state_default($state);
    $this->currency = config_get('uc_store.settings', 'uc_currency_code');
    $this->billing_country = config_get('uc_store.settings', 'uc_store_country');
    $this->delivery_country = config_get('uc_store.settings', 'uc_store_country');
    $this->created = REQUEST_TIME;
    $this->modified = REQUEST_TIME;
  }

  /**
   * Implements EntityInterface::id().
   */
  public function id() {}
  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {}
  /**
   * Implements EntityInterface::bundle().
   */
  public function bundle() {}
  /**
   * Implements EntityInterface::label().
   */
  public function label() {}
  /**
   * Implements EntityInterface::uri().
   */
  public function uri() {}
}