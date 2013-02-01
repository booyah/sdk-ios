//
//  PHAPIRequest.h
//  playhaven-sdk-ios
//
//  Created by Jesus Fernandez on 3/30/11.
//  Copyright 2011 Playhaven. All rights reserved.
//

#import <CommonCrypto/CommonHMAC.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

// TODO: Optimize the imports
#import "PHConnectionManager.h"

#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 60000
#import <AdSupport/AdSupport.h>
#endif
#endif

//@protocol PHConnectionManagerDelegate;
@protocol PHAPIRequestDelegate;

/**
 * @internal
 *
 * @brief Base class for API requests, generates valid request signatures based on the
 * device's GID, and checks for a valid X-PH-DIGEST response signature.
 **/
@interface PHAPIRequest : NSObject <PHConnectionManagerDelegate> {
    NSURL           *_URL;
    NSString        *_token, *_secret;
    NSDictionary    *_signedParameters;
    NSDictionary    *_additionalParameters;
    NSString        *_urlPath;
    int              _hashCode;

    BOOL theRightWayToStopTheRequestFromBeingSentTwiceIfItsActuallyNeeded;
//    NSURLConnection *_connection;
//    NSMutableData   *_connectionData;
//    NSURLResponse   *_response;

    id<PHAPIRequestDelegate> _delegate;
}

/**
 * Generates a URL-friendly b64 signature digest of \c string
 *
 * @param string
 *   The input string
 *
 * @return
 *   The b64 signature digest of \c string
 **/
+ (NSString *)base64SignatureWithString:(NSString *)string;

/**
 * Generates the expected X-PH-DIGEST response signature based on the response body, nonce, and app secret
 *
 * @param response
 *   The response
 *
 * @param nonce
 *   The nonce
 *
 * @param secret
 *   The secret
 *
 * @return
 *   The expected X-PH-DIGEST
 **/
+ (NSString *)expectedSignatureValueForResponse:(NSString *)response nonce:(NSString *)nonce secret:(NSString *)secret;

/**
 * Retrieves the PHID (otherwise known as the session token) from the pasteboard.
 * This value is used for GID/PHID-based device identification
 *
 * @return
 *   The session token
 **/
+ (NSString *)session;

/**
 * @name Opt Out Status
 **/
/*@{*/
/**
 * Gets and sets the UDID opt-out status. If \c YES and <tt>PH_USE_UNIQUE_IDENTIFIER == 1</tt>,
 * then the device's UDID will be sent with each request. Defaults to \c YES
 **/
+ (BOOL)optOutStatus;
+ (void)setOptOutStatus:(BOOL)yesOrNo;
/*@}*/

/**
 * @name Plugin Identifier
 **/
/*@{*/
/**
 * Gets and sets the plugin identifier. Third party plugins based on the iOS SDK
 * should set this to a value that is unique for each plugin version.
 * Defaults to <tt>ios-PH_SDK_VERSION</tt>
 **/
+ (NSString *)pluginIdentifier;
+ (void)setPluginIdentifier:(NSString *)identifier;
/*@}*/

/**
 * Returns a new PHAPIRequest instance with the given token and secret
 *
 * @param token
 *   The token
 *
 * @param secret
 *   The secret
 *
 * @return
 *   The PHAPIRequest instance
 **/
+ (id)requestForApp:(NSString *)token secret:(NSString *)secret;

/**
 * Returns an existing request with a hash code value of \c hashCode.
 * Used by the Unity3d plugin.
 *
 * @param hashCode
 *   The hashCode
 *
 * @return
 *   The request
 **/
+ (id)requestWithHashCode:(int)hashCode;

/**
 * Cancels all requests for a given delegate, typically used when a view
 * controller is set as request delegate, and that view controller might be
 * dismissed by the user while there are active API requests
 *
 * @param delegate
 *   The delegate
 **/
+ (void)cancelAllRequestsWithDelegate:(id<PHAPIRequestDelegate>)delegate;

/**
 * Cancels an existing request with a hash code value of \c hashCode.
 * Used by the Unity3d plugin.
 *
 * @param hashCode
 *   The hashCode
 *
 * @return
 *   An int that must mean something
 **/
+ (int)cancelRequestWithHashCode:(int)hashCode;

@property (nonatomic, readonly) NSString *token;   /**< API token for this request, value is set during initialization */
@property (nonatomic, readonly) NSString *secret;  /**< API secret for this request, value is set during initialization */
@property (nonatomic, copy)     NSString *urlPath; /**< API endpoint to use for this request, subclasses will override
                                                        this with a hard-coded value */
@property (nonatomic, readonly) NSURL    *URL;     /**< Lazily-initialized NSURL instance that contains a full request
                                                        URL with signed parameters */
@property (nonatomic, retain)   NSDictionary *additionalParameters; /**< Subclasses can override this implementation
                                                                         to add custom parameters to requests */
@property (nonatomic, readonly) NSDictionary *signedParameters;     /**< Lazily-initialized dictionary of base request
                                                                         parameters as well as necessary request signatures */
@property (nonatomic, assign)   int hashCode;      /**< Unique hash code identifying this request. Used by the Unity3d plugin */

@property (nonatomic, assign)   id<PHAPIRequestDelegate>  delegate; /**< Request delegate, see PHAPIRequestDelegate */

/**
 * URL-encoded parameter string using keys and values in self.signedParameters
 **/
- (NSString *)signedParameterString;

/**
 * Start the request if it has not already started
 **/
- (void)send;

/**
 * Cancel the request if it has already started
 **/
- (void)cancel;
@end

/**
 * @internal
 *
 * @brief Delegate protocol for getting information about API requests
 **/
@protocol PHAPIRequestDelegate <NSObject>
@optional
/**
 * The \c request finished loading
 *
 * @param request
 *   The request
 **/
- (void)requestDidFinishLoading:(PHAPIRequest *)request;

/**
 * The \c request completed successfully, has a valid response signature and returned \c responseData
 *
 * @param request
 *   The request
 *
 * @param responseData
 *   response data
 **/
- (void)request:(PHAPIRequest *)request didSucceedWithResponse:(NSDictionary *)responseData;

/**
 * The \c request failed, \c error will either be a PHError instance or a NSURLConnection error
 *
 * @param request
 *   The request
 *
 * @param error
 *   The error
 *
 * @sa PHConstants.h
 **/
- (void)request:(PHAPIRequest *)request didFailWithError:(NSError *)error;
@end
