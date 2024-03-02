program gerapov
  real,allocatable::x(:),y(:),z(:),vx(:),vy(:)
  real,allocatable:: angle_degree(:)
  real:: x_fat, y_fat, z_fat, vx_fat, vy_fat , angle_fat
  real :: comp
  integer :: nr,nc,n,mini
  integer:: garb_int_1, garb_int_2
  real:: garb_rea_3
  character::cor*20, cor_mini*20
  real:: auxiliar
  character ::a*20,fmtcam*60,fmtesf*60,arquivopov*12, fmtcyl*100, fmttri*100
  character :: define*20,leitura_nao_utilizada*20,var*20,ultimo*4
  real::valor
  integer::novo_pov = 0,i
  real::t
  real:: r, r_cyl = 0.6
  real::look_x, look_y, look_z
  real:: z_high
  open(10,file="posicoes.dat")
  ladox     =  480
  ladoy     =  120
  r         =  0.3
  look_z    =  18
  look_x    =  240
  look_y    =  60
  
  if(ladox<ladoy)then
    ladoz=ladox*1.8
  else
    ladoz=ladox*0.8
  end if
  write(*,*)"dimensoes do sistema:",ladox,ladoy,ladoz
  
  call system("rm ray*.pov")
  call system("ls ray*.png > toto")
  
  open(40,file="toto")
  do while(i==0)
    read(40,*,ERR=50,END=50)var
    ultimo(1:4) = var(4:7)
    read(ultimo,*)  novo_pov
  end do
  
  50 continue
  write(*,*)"ultimo que existe:",novo_pov,var
  do lup = 1, 1000
    if(lup > novo_pov)then
      write(arquivopov,'(a3,i4.4,a4)')'ray',lup,'.pov'
      open(30, file = arquivopov)
      fmtcam = '(a16,f8.3,a1,f8.3,a1,f8.3,a9,f8.3,a1,f8.3,a1,f8.3,a2)'
      write(30,*) '#include "colors.inc"'
      write(30,*) '// tempo:',t
      write(30,fmtcam)'camera{location<',look_x,',',2*10*look_z,',',look_y,'>look_at<',look_x,',',r,',',look_y,'>}'
      write(30,'(a14,f8.3,a1,f8.3,a1,f8.3,a13)')'light_source{<',0.,',',4.*ladoz,',',-2.*ladox,'>color White}'
      write(30,'(a10,f8.3,a35)') 'plane { y,', 0., ' pigment{color rgb<0.1, 0.1, 0.1>}}'
      fmtesf = '(a8,f8.4,a1,f5.2,a1,f8.4,a2,f8.3,a23,a13,a30)'
      write(30,*)"disc{<",look_x,",",r,",",look_y,">, <0.0, 1.0, 0.0>, &
&     ",look_z,", pigment{ color rgb<0.5, 0.5 ,0.5>}}"
    
      read(10,*,END=100)t,n,mini
      write(*,*)lup, n, (mini-1)*n
      
      do i = 1, n
        allocate(x(mini),y(mini),z(mini),vx(mini),vy(mini),angle_degree(mini))
        read(10,*)x_fat,y_fat,vx_fat,vy_fat, garb_int_1, garb_int_2, garb_rea_3
        angle_fat=atan2(vy_fat,vx_fat)
        do while(angle_fat > 6.2830)
          angle_fat = angle_fat - 6.2830
        end do
        do while(angle_fat < 0.0000)
          angle_fat = angle_fat + 6.2830
        end do
        do j = 1, mini
          read(10,*)x(j),y(j),vx(j),vy(j), garb_int_1, garb_int_2, garb_rea_3
          angle_degree(j) = atan2(vy(j),vx(j))
          do while(angle_degree(j) > 6.2830)
            angle_degree(j) = angle_degree(j) - 6.2830
          end do
          do while(angle_degree(j) < 0.0000)
            angle_degree(j) = angle_degree(j) + 6.2830
          end do
          angle_degree(j) = 360*(angle_degree(j))/(6.2830)
        end do
        z = r
        x = x + 240
        y = y + 60

        z_fat = r
        x_fat = x_fat + 240
        y_fat = y_fat + 60

        z_high = 1.5*r_cyl
        
        angle_fat = 360*(angle_fat)/(6.2830)
        write(cor,'(a1,i3,a9)')'<',int(angle_fat),',1.0,0.5>'
        write(30,fmtesf)'sphere{<',x_fat,',',z_high,',',y_fat,'>,',r, &
&         'pigment{color CHSL2RGB(',cor,')}finish{phong 1}}'
        

        fmtcyl = '(a11,f8.4,a1,f5.2,a1,f8.4,a4,f8.4,a1,f5.2,a1,f8.4,a2,f8.3,a25,a13,a20)'
        fmttri = '(a11,f8.4,a1,f5.2,a1,f8.4,a4,f8.4,a1,f5.2,a1, &
&         f8.4,a4,f8.4,a1,f5.2,a1,f8.4,a25,a13,a20)'
!" 
        do j = 1, mini-1
          write(cor_mini,'(a1,i3,a9)')'<',int(angle_degree(j)),',1.0,0.5>'
          write(30,fmttri)'triangle{< ',x_fat,',',z_high-0.1,',',y_fat,'>, <', &
&           x(j),',',z_high-0.1,',',y(j),'>, <',x(j+1),',',z_high-0.1,',',y(j+1),'> pigment{color CHSL2RGB(', &
&           cor,')}}'
          write(30,fmtcyl)'cylinder{< ',x(j),',',z(j),',',y(j),'>, <', &
&           x(j+1),',',z(j+1),',',y(j+1),'>,',r_cyl-0.1,' pigment{color CHSL2RGB(', &
&           cor,')}}'
          write(30,fmtesf)'sphere{<',x(j),',',z(j),',',y(j),'>,',r_cyl, &
&           'pigment{color CHSL2RGB(',cor_mini,')}finish{phong 1}}'
          
        end do
        write(cor_mini,'(a1,i3,a9)')'<',int(angle_degree(mini)),',1.0,0.5>'
        write(30,fmttri)'triangle{< ',x_fat,',',z_high-0.1,',',y_fat,'>, <',x(mini),',', &
&         z_high-0.1,',',y(mini),'>, <',x(1),',',z_high-0.1,',',y(1),'> pigment{color CHSL2RGB(',cor,')}}'
        write(30,fmtcyl)'cylinder{< ',x(mini),',',z(mini),',',y(mini),'>, <', &
&         x(1),',',z(1),',',y(1),'>,',r_cyl-0.1,' pigment{ color CHSL2RGB(', &
&         cor,')}}'
        write(30,fmtesf)'sphere{<',x(mini),',',z(mini),',',y(mini),'>,',r_cyl, &
&         'pigment{color CHSL2RGB(',cor_mini,')}finish{phong 1}}'
        deallocate(x,y,z,vx,vy,angle_degree)
      end do    
      close(30)
    else
      read(10,*,END=100)t,n,mini
      write(*,*)lup, n, (mini-1)*n, " skip "
      do i = 1, n
        allocate(x(mini),y(mini),z(mini),vx(mini),vy(mini))
        read(10,*)x_fat,y_fat,vx_fat,vy_fat, garb_int_1, garb_int_2, garb_rea_3
        do j = 1, mini
          read(10,*)x(j),y(j),vx(j),vy(j), garb_int_1, garb_int_2, garb_rea_3
        end do
        deallocate(x,y,z,vx,vy)
      end do
    end if
end do
100 continue
call system("rm toto")
end program gerapov
