<?php

/**
 * @file
 * Weight quote shipping method administration menu items.
 */

/**
 * Configures the store default product shipping rates.
 *
 * @see uc_weightquote_admin_method_edit_form_submit()
 * @see uc_weightquote_admin_method_edit_form_delete()
 * @ingroup forms
 */
function uc_weightquote_admin_method_edit_form($form, &$form_state, $mid = 0) {
  if ($mid && ($method = db_query("SELECT * FROM {uc_weightquote_methods} WHERE mid = :mid", array(':mid' => $mid))->fetchObject())) {
    $form['mid'] = array(
      '#type' => 'value',
      '#value' => $mid,
    );
  }
  else {
    $method = (object) array(
      'title' => '',
      'label' => '',
      'base_rate' => '',
      'product_rate' => '',
    );
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipping method title'),
    '#description' => t('The name shown to administrators distinguish this method from other weightquote methods.'),
    '#default_value' => $method->title,
    '#required' => TRUE,
  );
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Line item label'),
    '#description' => t('The name shown to the customer when they choose a shipping method at checkout.'),
    '#default_value' => $method->label,
    '#required' => TRUE,
  );
  $form['base_rate'] = array(
    '#type' => 'uc_price',
    '#title' => t('Base price'),
    '#description' => t('The starting price for weight-based shipping costs.'),
    '#default_value' => $method->base_rate,
    '#required' => TRUE,
  );
  $form['product_rate'] = array(
    '#type' => 'uc_price',
    '#title' => t('Default cost adjustment per !unit', array('!unit' => config_get('uc_store.settings', 'uc_weight_unit'))),
    '#description' => t('The amount per weight unit to add to the shipping cost for an item.'),
    '#default_value' => $method->product_rate,
    '#required' => TRUE,
    '#field_suffix' => t('per @unit', array('@unit' => config_get('uc_store.settings', 'uc_weight_unit'))),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  if (isset($form['mid'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#validate' => array(),
      '#submit' => array('uc_weightquote_admin_method_edit_form_delete'),
    );
  }

  return $form;
}

/**
 * Helper function to delete a weightquote method.
 *
 * @see uc_weightquote_admin_method_edit_form()
 */
function uc_weightquote_admin_method_edit_form_delete($form, &$form_state) {
  backdrop_goto('admin/store/settings/quotes/weightquote/' . $form_state['values']['mid'] . '/delete');
}

/**
 * Form submission handler for uc_weightquote_admin_method_edit_form().
 *
 * @see uc_weightquote_admin_method_edit_form()
 */
function uc_weightquote_admin_method_edit_form_submit($form, &$form_state) {
  if (isset($form_state['values']['mid'])) {
    backdrop_write_record('uc_weightquote_methods', $form_state['values'], 'mid');
    backdrop_set_message(t('Weight quote shipping method was updated.'));
    $form_state['redirect'] = 'admin/store/settings/quotes/methods';
  }
  else {
    backdrop_write_record('uc_weightquote_methods', $form_state['values']);

    // Ensure Rules picks up the new condition.
    entity_info_cache_clear();

    backdrop_set_message(t('Created and enabled new weight quote shipping method.'));
    $form_state['redirect'] = 'admin/store/settings/quotes/manage/get_quote_from_weightquote_' . $form_state['values']['mid'];
  }
}

/**
 * Confirms deletion of a weight-based shipping method.
 *
 * @see uc_weightquote_admin_method_confirm_delete_submit()
 * @ingroup forms
 */
function uc_weightquote_admin_method_confirm_delete($form, &$form_state, $mid) {
  $form['mid'] = array(
    '#type' => 'value',
    '#value' => $mid,
  );

  return confirm_form($form, t('Do you want to delete this shipping method?'),
    'admin/store/settings/quotes/methods',
    t('This will remove the shipping method and the product-specific overrides (if applicable). This action can not be undone.'),
    t('Delete'));
}

/**
 * Form submission handler for uc_weightquote_admin_method_confirm_delete().
 *
 * @see uc_weightquote_admin_method_confirm_delete()
 */
function uc_weightquote_admin_method_confirm_delete_submit($form, &$form_state) {
  $mid = $form_state['values']['mid'];

  db_delete('uc_weightquote_methods')
    ->condition('mid', $mid)
    ->execute();
  db_delete('uc_weightquote_products')
    ->condition('mid', $mid)
    ->execute();

  rules_config_delete(array('get_quote_from_weightquote_' . $mid));

  backdrop_set_message(t('Weight quote shipping method deleted.'));
  $form_state['redirect'] = 'admin/store/settings/quotes/methods';
}
