**************************************
*   UberCart MailChimp Integration   *
**************************************
*****Sponsored By*****

Adam A. Gregory (http://AdamAGregory.com) and Marketing Ministries(http://MarketingMinistries.com) A full service Ad agency that develops on Drupal.

*****Description*****
This module integrates UberCart with the Mailchimp API's ECommerce 360 feature for tracking store statistics for email campaigns sent via MailChimp. It also adds a checkout pane to the shopping cart that allows customers to subscribe to lists during the checkout proccess.

*****Features*****

    * Sends order info to MailChimp for campaign tracking
    * debugging and logging options
    * Options for Cookie storage length
    * Adds tokens to the MailChimp Module to get first and last name from UC orders

*****Requires*****

    * Ubercart
    * MailChimp Module or Email Marketing Framework with MailChimp support
    * An account with Mailchimp

*****Installation/usage*****

Obviously you must first set up and configure the Ubercart and MailChimp/EMF Modules. Once done you will need to set up your campaign in MailChimp to add the ECommerce 360 tracking (View The MailChimp Post on ECommerce). Once that is done and you send your campaign this module will start tracking and sending the data back to MailChimp

You can also set up a checkout pane that will provide a check bx for users to check/uncheck to subscribe to lists of your choosing during the checkout process.
*****Notes*****    
    ***Settings Pages***
    There are two settings pages for this module.
    One is in admin/store/settings/mailchimp, this is where you can set up some of the Ecommerce 360 tracking items.
    The other is admin/store/settings/checkout/edit/panes, this is where you would set up the checkout pane and configure each of the lists you want people checking out to be able to use.
        ****You must enable the Checkout pane on this page or it won't show up.
        

The two modules(MailChimp Stand alone & EMF w/ mailchimp) create issues if you try to install them together. So only pick one of them to use with this module.
