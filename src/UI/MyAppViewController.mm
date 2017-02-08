//
//  MenuViewController.m
//  Created by lukasz karluk on 12/12/11.
//  Updated for iOS8+ by Andreas Borg 11/18/15
//
// Adapted by Tomas Jasevicius, found at:
// OpenframeWorks for ios v0.9.8 example called "iosNativeExample".
//

#import "MyAppViewController.h"

#import "arAppViewController.h"
#import "arApp.h"

@interface MyAppViewController()
@property UIScrollView* containerView;
@end

@implementation MyAppViewController



- (UIButton*) makeButtonWithFrame:(CGRect)frame 
                          andText:(NSString*)text {
    UIFont *font;
    font = [UIFont fontWithName:@"Mono" size:30];
    
    UILabel *label;
    label = [[ UILabel alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)];
    label.backgroundColor = [UIColor colorWithWhite:1 alpha:0.95];
    label.textColor = [UIColor colorWithWhite:0 alpha:1];
    label.text = [text uppercaseString];
    label.textAlignment = NSTextAlignmentCenter;
    label.font = font;
    label.userInteractionEnabled = NO;
    label.exclusiveTouch = NO;
    
    UIButton* button = [[UIButton alloc] initWithFrame:frame];
    [button setBackgroundColor:[UIColor clearColor]];
    [button addSubview:label];
    
    return button;
}

- (void)loadView {
    [super loadView];
    
    self.view.backgroundColor = [UIColor grayColor];
    
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    
    CGRect scrollViewFrame = CGRectMake(0.f,
                                        0.f,
                                        screenRect.size.width,
                                        screenRect.size.height);
    
    self.containerView = [[UIScrollView alloc] initWithFrame:scrollViewFrame];
    self.containerView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    self.containerView.backgroundColor = [UIColor clearColor];
    self.containerView.showsHorizontalScrollIndicator = NO;
    self.containerView.showsVerticalScrollIndicator = YES;
    self.containerView.alwaysBounceVertical = YES;
    [self.view addSubview:self.containerView];

    NSString *buttonName = @"Tap to begin";
    NSInteger buttonY = 0;
    NSInteger buttonHeight = screenRect.size.height;
    CGRect buttonRect = CGRectMake(0, 0, screenRect.size.width, buttonHeight);
    
    UIButton *button;
    
    button = [self makeButtonWithFrame:CGRectMake(0, buttonY, buttonRect.size.width, buttonRect.size.height)andText:(buttonName)];
    
    [self.containerView addSubview:button];
    
    [button addTarget:self action:@selector(buttonPressed:) forControlEvents:UIControlEventTouchUpInside];

}

- (void)buttonPressed:(id)sender {
    arAppViewController *viewController;
    viewController = [[arAppViewController alloc] initWithFrame:[[UIScreen mainScreen] bounds]
                                                                 app:new arApp()];
    
    [self.navigationController pushViewController:viewController animated:YES];
    self.navigationController.navigationBar.topItem.title = @"arApp";
}


//The rest of the code is left unchanged from example//

// iOS7-
//borg
- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return NO;
/*
    BOOL bRotate = NO;
    bRotate = bRotate || (toInterfaceOrientation == UIInterfaceOrientationPortrait);
    bRotate = bRotate || (toInterfaceOrientation == UIInterfaceOrientationPortraitUpsideDown);
    return bRotate;
    */
}



// iOS8+ version of willAnimateRotationToInterfaceOrientation
//borg
- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator {
    //this only reports accurately when this is actually in view. If you enter an app,
    //then rotate and exit, it will not be updating correctly..hence still needs
    //separate updateLayout checking active rotation
    cout<<"MyAppViewController - w "<<size.width<<" h "<<size.height<<endl;;
    
    [self updateLayout];
    
}
-(void)viewWillAppear:(BOOL)animated{
    [self updateLayout];
}

-(void)updateLayout{
    string currentOrientation;

    CGRect rect;
    int longSide = MAX([[UIScreen mainScreen] bounds].size.width,[[UIScreen mainScreen] bounds].size.height);
    int shortSide = MIN([[UIScreen mainScreen] bounds].size.width,[[UIScreen mainScreen] bounds].size.height);
    if (UIDeviceOrientationIsLandscape([UIDevice currentDevice].orientation)){
         // code for landscape orientation
        currentOrientation = "Landscape";
        rect = CGRectMake(0,0,longSide,shortSide);
    }else{
        currentOrientation = "Portrait";
        rect = CGRectMake(0,0,shortSide,longSide);
    }
    
    //UIDeviceOrientationIsLandscape not same results as ofGetOrientation()
    
    cout<<"MyAppViewController updateLayout - w "<<rect.size.width<<" h "<<rect.size.height<<", currentOrientation "<<currentOrientation<<endl;
    
    self.view.frame = rect;
    self.containerView.frame = rect;
    
    //this is not elegant...create bespoke classes instead
    for(UIView* btn in self.containerView.subviews){
        if([btn isKindOfClass:[UIButton class]]){
            btn.frame = CGRectMake(btn.frame.origin.x, btn.frame.origin.y, rect.size.width, btn.frame.size.height);
            for(UIView* label in btn.subviews){
                if([label isKindOfClass:[UILabel class]]){
                    label.frame = CGRectMake(label.frame.origin.x, label.frame.origin.y, rect.size.width, label.frame.size.height);
                }
            }
        }
    
    }
    self.containerView.contentSize = CGSizeMake(rect.size.width, self.containerView.contentSize.height);
}

@end
