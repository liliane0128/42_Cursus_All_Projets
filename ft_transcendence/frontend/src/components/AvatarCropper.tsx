import { useState, useCallback } from "react";
import Cropper from "react-easy-crop";
import { X, Check } from "lucide-react";

interface Props {
  imageSrc: string;
  onCancel: () => void;
  onCrop: (blob: Blob) => void;
}

async function getCroppedImg(imageSrc: string, croppedAreaPixels: any): Promise<Blob> {
  const image = new Image();
  image.crossOrigin = "anonymous";
  image.src = imageSrc;
  await new Promise(resolve => { image.onload = resolve; });
  const canvas = document.createElement("canvas");
  canvas.width = croppedAreaPixels.width;
  canvas.height = croppedAreaPixels.height;
  const ctx = canvas.getContext("2d")!;
  ctx.drawImage(
    image,
    croppedAreaPixels.x, croppedAreaPixels.y,
    croppedAreaPixels.width, croppedAreaPixels.height,
    0, 0,
    croppedAreaPixels.width, croppedAreaPixels.height
  );
  return new Promise(resolve => canvas.toBlob(blob => resolve(blob!), "image/jpeg", 0.9));
}

export default function AvatarCropper({ imageSrc, onCancel, onCrop }: Props) {
  const [crop, setCrop] = useState({ x: 0, y: 0 });
  const [zoom, setZoom] = useState(1);
  const [croppedAreaPixels, setCroppedAreaPixels] = useState<any>(null);

  const onCropComplete = useCallback((_: any, croppedAreaPixels: any) => {
    setCroppedAreaPixels(croppedAreaPixels);
  }, []);

async function handleConfirm() {
  console.log("handleConfirm called", croppedAreaPixels);
  try {
    console.log("calling getCroppedImg");
    const blob = await getCroppedImg(imageSrc, croppedAreaPixels);
    console.log("blob:", blob);
    onCrop(blob);
  } catch(err) {
    console.error("crop error:", err);
  }
}

  return (
    <div className="fixed inset-0 bg-black/60 z-50 flex items-center justify-center p-4">
      <div className="bg-white rounded-xl overflow-hidden w-full max-w-sm">
        <div className="px-4 py-3 border-b border-gray-100 flex items-center justify-between">
          <p className="text-[13px] font-semibold">Crop avatar</p>
          <button onClick={onCancel} className="text-gray-400 hover:text-gray-900 transition-colors">
            <X size={16} />
          </button>
        </div>
        <div className="relative h-64 bg-gray-900">
          <Cropper
            image={imageSrc}
            crop={crop}
            zoom={zoom}
            aspect={1}
            cropShape="round"
            onCropChange={setCrop}
            onZoomChange={setZoom}
            onCropComplete={onCropComplete}
          />
        </div>
        <div className="px-4 py-3">
          <input type="range" min={1} max={3} step={0.1} value={zoom}
            onChange={e => setZoom(Number(e.target.value))}
            className="w-full accent-gray-900" />
        </div>
        <div className="px-4 pb-4 flex gap-2">
          <button onClick={onCancel}
            className="flex-1 py-2 rounded-lg border border-gray-200 text-[12px] text-gray-600 hover:bg-gray-50 transition-colors">
            Cancel
          </button>
          <button onClick={handleConfirm}
            className="flex-1 py-2 rounded-lg bg-gray-900 text-white text-[12px] font-medium hover:bg-gray-700 transition-colors flex items-center justify-center gap-2">
            <Check size={13} /> Apply
          </button>
        </div>
      </div>
    </div>
  );
}
