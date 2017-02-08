//
//  CustomAppViewController.m
//  Created by lukasz karluk on 8/02/12.
//
// Adapted by Tomas Jasevicius from:
// OpenframeWorks for ios v0.9.8 example called "iosNativeExample".
//

#import "arAppViewController.h"
#import "ofxiOSExtras.h"
#import "ofAppiOSWindow.h"

@implementation arAppViewController

- (id) initWithFrame:(CGRect)frame app:(ofxiOSApp *)app {
    
    ofxiOSGetOFWindow()->setOrientation(OF_ORIENTATION_DEFAULT);   //-- default portait orientation.
    
    return self = [super initWithFrame:frame app:app];
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return NO;
}

@end
