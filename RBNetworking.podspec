

Pod::Spec.new do |s|
  s.name             = 'RBNetworking'
  s.version="1.2.0"
  s.summary          = 'A short description of RBNetworking.'
  s.description      = <<-DESC
TODO: Add long description of the pod here.
                       DESC

  s.homepage         = 'https://github.com/RooboMobile/RBNetworking'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'baxiang' => 'baxiang@roobo.com' }
  s.source           = { :git => 'https://github.com/RooboMobile/RBNetworking.git', :tag => s.version.to_s }
  s.ios.deployment_target = '7.0'
  s.source_files = 'RBNetworking/*.{h,m}'
  s.dependency 'AFNetworking'
end
