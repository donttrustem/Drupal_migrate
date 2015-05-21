<?php

/**
 * @file perla.inc
 * XML import from La Perla.
 */

/**
 * XML import from La Perla Class.
 */
class LaPerlaxmlimport extends XMLMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('XML feed of La Perla Properties.');

    // There isn't a consistent way to automatically identify appropriate
    // "fields" from an XML feed, so we pass an explicit list of source fields.

    $fields = array(
      'id' => t('ID'),
      'lang_type' => t('Language'),
      'field_ref_remote' => t('Ref'),
      'field_location_prop_remote' => t('Location'),
      'title_remote' => t('Title'),
      'field_price_remote' => t('Price'),
      'field_status_remote' => t('Status'),
      'field_listing_type_remote' => t('Listing Type'),
      'field_property_type_remote' => t('Property Type'),
      'field_cee_epc_remote' => t('CEE/EPC'),
      'field_bedrooms_remote' => t('Bedrooms'),
      'field_bathrooms_remote' => t('Bathrooms'),
      'field_sq_ft_remote' => t('Dwelling Size'),
      'field_lot_size_remote' => t('Plot Size'),
      'field_interior_features_remote' => t('Home Features'),
      'body_remote' => t('Description'),
      'field_image_remote' => t('Property Photos'),
    );

    // The source ID here is the one retrieved from the XML listing URL, and
    // used to identify the specific item's URL.
    $this->map = new MigrateSQLMap($this->machineName, array(
      'ID' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Source ID',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Source list URL.
    $list_url = 'http://xml2u.com/Xml/La%20Perla%20estates_1426/4244_Default.xml';

    // Each ID retrieved from the list URL will be plugged into :id in the
    // item URL to fetch the specific objects.
    // @todo: Add langtype for importing translated content.
    $item_url = 'http://www.laperlaestates.com/view-all-properties?view=details&amp;catitemid=:id';

    // Commented out for testing.

    //$items_url = 'http://xml2u.com/Xml/La%20Perla%20estates_1426/4244_Default.xml';
    //$item_xpath = '/document/Clients/Client/properties/Property';  // relative to document
    // $item_ID_xpath = 'propertyid';         // relative to item_xpath and gets assembled
                                             // into full path /producers/producer/sourceid

    // $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_ID_xpath);
    //$this->source = new MigrateSourceMultiItems($items_class, $fields);
    //$this->destination = new MigrateDestinationNode('property');

    // We use the MigrateSourceList class for any source where we obtain the
    // list of IDs to process separately from the data for each item. The
    // listing and item are represented by separate classes, so for example we
    // could replace the XML listing with a file directory listing, or the XML
    // item with a JSON item.
    $this->source = new MigrateSourceList(new MigrateListXML($list_url),
      new MigrateItemXML($item_url), $fields);

    $this->destination = new MigrateDestinationNode('Property');

    // TIP: Note that for XML sources, in addition to the source field passed to
    // addFieldMapping (the name under which it will be saved in the data row
    // passed through the migration process) we specify the Xpath used to retrieve
    // the value from the XML.

    $this->addFieldMapping('field_ref_remote', 'field_ref')
      ->xpath('/document/Clients/Client/properties/Property/reference');

    $this->addFieldMapping('field_location_prop_remote', 'field_location_prop')
      ->xpath('/document/Clients/Client/properties/Property/Address/location');

    $this->addFieldMapping('title_remote', 'title')
      ->xpath('/document/Clients/Client/properties/Property/Description/title');

    $this->addFieldMapping('field_price_remote', 'field_price')
      ->xpath('/document/Clients/Client/properties/Property/Address/Price/price');

    $this->addFieldMapping('field_status_remote', 'field_status')
      ->xpath('/document/Clients/Client/properties/Property/Address/Price/price');

    $this->addFieldMapping('field_listing_type_remote', 'field_listing')
      ->xpath('/document/Clients/Client/properties/Property/Description/propertyType');

    $this->addFieldMapping('field_cee_epc_remote', 'field_cee_epc')
      ->xpath('/document/Clients/Client/properties/Property/EPC');

    $this->addFieldMapping('field_cee_epc_remote', 'field_cee_epc')
      ->xpath('/document/Clients/Client/properties/Property/GEC');

    $this->addFieldMapping('field_bedrooms_remote', 'field_bedrooms')
      ->xpath('/document/Clients/Client/properties/Property/Description/bedrooms');

    $this->addFieldMapping('field_bathrooms_remote', 'field_bathrooms')
      ->xpath('/document/Clients/Client/properties/Property/Description/fullBathrooms');

    $this->addFieldMapping('field_sq_ft_remote', 'field_sq_ft_')
      ->xpath('/document/Clients/Client/properties/Property/Description/FloorSize/floorSize');

    $this->addFieldMapping('field_lot_size_remote', 'field_lot_size')
      ->xpath('/document/Clients/Client/properties/Property/Description/PlotSize/plotSize');

    $this->addFieldMapping('field_interior_features_remote', 'field_interior_features')
      ->xpath('/document/Clients/Client/properties/Property/Description/Features');

    $this->addFieldMapping('body_remote', 'body')
      ->arguments(array('format' => 'filtered_html'))
      ->xpath('/document/Clients/Client/properties/Property/Description/description');

    $this->addFieldMapping('field_image_remote:file_replace')
      ->defaultValue(MigrateFile::FILE_EXISTS_REUSE); //FILE_EXISTS_REUSE is in the MigrateFile class


    $this->addFieldMapping('field_image_remote:source_dir', 'field_image')
      ->arguments(array('format' => 'filtered_html'))
      ->xpath('/document/Clients/Client/properties/Property/images/image/image');

    $this->addUnmigratedSources($unmapped_sources);

    // Declare unmapped destination fields.
    $unmapped_destinations = array(
      'revision_uid',
      'changed',
      'status',
      'promote',
      'sticky',
      'revision',
      'log',
      'language',
      'tnid',
      'is_new',
      'body:language',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $ctype = (string)$row->XML->Type;

    // Set variable for return code.
    $ret = FALSE;
  }

  function perla_migrate_api() {
    $api = array(
      'api' => 2,

      // Give the group a human readable title.
      'groups' => array(
        'vision' => array(
          'title' => t('Perla'),
        ),
      ),

      'migrations' => array(
        'LaPerla' => array(
          'class_name' => 'LaPerlaxmlimport',
        ),
      ),
    );

    return $api;
  }
}
