<?php

/**
 * @file perla.inc
 * XML import from La Perla.
 */

/**
 * XML import from La Perla Class.
 */
class LaPerlaxmlimport extends XMLMigration {
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('XML feed of La Perla Properties.');

    // There isn't a consistent way to automatically identify appropriate
    // "fields" from an XML feed, so we pass an explicit list of source fields.

    $source_fields = array(
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
      new MigrateItemXML($item_url), $source_fields);

    $this->destination = new MigrateDestinationNode('property');
dd($this->destination);
    // TIP: Note that for XML sources, in addition to the source field passed to
    // addFieldMapping (the name under which it will be saved in the data row
    // passed through the migration process) we specify the Xpath used to retrieve
    // the value from the XML.

    $this->addFieldMapping('field_ref', 'field_ref_remote')
      ->xpath('/document/Clients/Client/properties/Property/reference');

    $this->addFieldMapping('field_location_prop', 'field_location_prop_remote')
      ->xpath('/document/Clients/Client/properties/Property/Address/location');

    $this->addFieldMapping('title', 'title_remote')
      ->xpath('/document/Clients/Client/properties/Property/Description/title');

    $this->addFieldMapping('field_price', 'field_price_remote')
      ->xpath('/document/Clients/Client/properties/Property/Address/Price/price');

    $this->addFieldMapping('field_status', 'field_status_remote')
      ->xpath('/document/Clients/Client/properties/Property/category');

    $this->addFieldMapping('field_listing_type', 'field_listing_remote')
      ->xpath('/document/Clients/Client/properties/Property/Description/propertyType');

    $this->addFieldMapping('field_cee_epc', 'field_cee_epc_remote')
      ->xpath('/document/Clients/Client/properties/Property/EPC');

    $this->addFieldMapping('field_bedrooms', 'field_bedrooms_remote')
      ->xpath('/document/Clients/Client/properties/Property/Description/bedrooms');

    $this->addFieldMapping('field_bathrooms', 'field_bathrooms_remote')
      ->xpath('/document/Clients/Client/properties/Property/Description/fullBathrooms');

    $this->addFieldMapping('field_sq_ft_', 'field_sq_ft_remote')
      ->xpath('/document/Clients/Client/properties/Property/Description/FloorSize/floorSize');

    $this->addFieldMapping('field_lot_size', 'field_lot_size_remote')
      ->xpath('/document/Clients/Client/properties/Property/Description/PlotSize/plotSize');

    $this->addFieldMapping('field_interior_features', 'field_interior_features_remote')
      ->xpath('/document/Clients/Client/properties/Property/Description/Features');

    $this->addFieldMapping('body', 'body_remote')
      ->xpath('/document/Clients/Client/properties/Property/Description/description');

    $this->addFieldMapping('body_remote:format', 'filtered_html');

    $this->addFieldMapping('field_image:file_replace')
      ->defaultValue(MigrateFile::FILE_EXISTS_REUSE); //FILE_EXISTS_REUSE is in the MigrateFile class


    $this->addFieldMapping('field_image', 'field_image_remote:source_dir')
      ->xpath('/document/Clients/Client/properties/Property/images/image/image');

    $this->addFieldMapping('field_image:format', 'filtered_html');

    $unmapped_sources = array();
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
}
